#include "gamestate.h"
#include <stdexcept>

GameState::GameState(int numPlayers, Language language)
    : numPlayers(numPlayers), currentPlayerIndex(0), currentPhase(GamePhase::SETUP),
      consecutiveYields(0), jesterActive(false), jestersUsed(0), language(language)
{
    if (numPlayers < 1 || numPlayers > 4)
    {
        const QString message = trText("Number of players must be between 1 and 4",
                                       "O número de jogadores deve estar entre 1 e 4");
        throw std::invalid_argument(message.toStdString());
    }
}

void GameState::setupGame()
{
    Card::setLanguage(language);
    // Create decks
    tavernDeck = Deck::createTavernDeck(numPlayers);
    castleDeck = Deck::createCastleDeck();

    // Create players with appropriate hand sizes
    int maxHandSize = 8; // Default for 1 player
    if (numPlayers == 2)
        maxHandSize = 7;
    else if (numPlayers == 3)
        maxHandSize = 6;
    else if (numPlayers == 4)
        maxHandSize = 5;

    players.clear();
    for (int i = 0; i < numPlayers; i++)
    {
        players.append(Player(i, maxHandSize, language));
    }

    dealInitialHands();
    revealFirstEnemy();

    currentPhase = GamePhase::PLAY_CARD;
    addToLog(trText("Game started! %1 player(s)", "Jogo iniciado! %1 jogador(es)").arg(numPlayers));
    addToLog(trText("First enemy: %1", "Primeiro inimigo: %1").arg(currentEnemy->toString()));
}

void GameState::dealInitialHands()
{
    for (Player &player : players)
    {
        for (int i = 0; i < player.getMaxHandSize(); i++)
        {
            if (!tavernDeck.isEmpty())
            {
                player.addCardToHand(tavernDeck.draw());
            }
        }
    }
}

void GameState::revealFirstEnemy()
{
    if (!castleDeck.isEmpty())
    {
        Card enemyCard = castleDeck.draw();
        currentEnemy = std::make_unique<Enemy>(enemyCard);
    }
}

bool GameState::playCards(int playerIndex, const QVector<int> &cardIndices)
{
    if (playerIndex != currentPlayerIndex)
    {
        return false;
    }

    if (currentPhase != GamePhase::PLAY_CARD)
    {
        return false;
    }

    Player &player = players[playerIndex];
    QVector<Card> cardsToPlay;

    // Collect cards (in reverse order to maintain indices)
    QVector<int> sortedIndices = cardIndices;
    std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<int>());

    for (int index : sortedIndices)
    {
        if (index >= 0 && index < player.getHandSize())
        {
            cardsToPlay.prepend(player.playCard(index));
        }
    }

    // Validate combo
    if (!isValidCombo(cardsToPlay))
    {
        // Return cards to hand
        for (const Card &card : cardsToPlay)
        {
            player.addCardToHand(card);
        }
        return false;
    }

    // Store played cards
    currentPlay.clear();
    currentPlay.cards = cardsToPlay;
    currentPlay.totalAttackValue = calculateComboAttackValue(cardsToPlay);

    // Analyze suits
    for (const Card &card : cardsToPlay)
    {
        if (!card.getIsJester())
        {
            Suit suit = card.getSuit();
            int value = card.getValue();

            if (suit == Suit::HEARTS)
            {
                currentPlay.hasHearts = true;
                currentPlay.heartsValue += value;
            }
            else if (suit == Suit::DIAMONDS)
            {
                currentPlay.hasDiamonds = true;
                currentPlay.diamondsValue += value;
            }
            else if (suit == Suit::CLUBS)
            {
                currentPlay.hasClubs = true;
                currentPlay.clubsValue += value;
            }
            else if (suit == Suit::SPADES)
            {
                currentPlay.hasSpades = true;
                currentPlay.spadesValue += value;
            }
        }
    }

    // Check if Jester was played
    for (const Card &card : cardsToPlay)
    {
        if (card.getIsJester())
        {
            jesterActive = true;
            currentEnemy->cancelImmunity();
            addToLog(player.getName() + trText(" played Jester - enemy immunity cancelled!",
                                               " jogou o Bobo - imunidade do inimigo cancelada!"));
        }
    }

    // Log the play
    QString playLog = player.getName() + trText(" played: ", " jogou: ");
    for (const Card &card : cardsToPlay)
    {
        playLog += card.toString() + ", ";
    }
    playLog += trText(" (Attack: %1)", " (Ataque: %1)").arg(currentPlay.totalAttackValue);
    addToLog(playLog);

    player.setYielded(false);
    consecutiveYields = 0;

    currentPhase = GamePhase::ACTIVATE_POWER;
    return true;
}

bool GameState::yieldTurn(int playerIndex)
{
    if (playerIndex != currentPlayerIndex)
    {
        return false;
    }

    if (!canYield(playerIndex))
    {
        return false;
    }

    Player &player = players[playerIndex];
    player.setYielded(true);
    consecutiveYields++;

    addToLog(player.getName() + trText(" yielded their turn", " cedeu o turno"));

    currentPhase = GamePhase::SUFFER_DAMAGE;
    return true;
}

void GameState::activateSuitPowers()
{
    if (currentPhase != GamePhase::ACTIVATE_POWER)
    {
        return;
    }

    // Check enemy immunity
    bool enemyImmune = currentEnemy->getImmunityActive();
    Suit enemySuit = currentEnemy->getSuit();

    // Activate Hearts first (if present with Diamonds, Hearts goes first)
    if (currentPlay.hasHearts)
    {
        if (!enemyImmune || enemySuit != Suit::HEARTS)
        {
            activateHearts(currentPlay.heartsValue);
        }
        else
        {
            addToLog(trText("Hearts power blocked by enemy immunity!",
                            "Poder de Copas bloqueado pela imunidade do inimigo!"));
        }
    }

    // Then Diamonds
    if (currentPlay.hasDiamonds)
    {
        if (!enemyImmune || enemySuit != Suit::DIAMONDS)
        {
            activateDiamonds(currentPlay.diamondsValue);
        }
        else
        {
            addToLog(trText("Diamonds power blocked by enemy immunity!",
                            "Poder de Ouros bloqueado pela imunidade do inimigo!"));
        }
    }

    // Clubs (affects damage calculation)
    if (currentPlay.hasClubs)
    {
        if (!enemyImmune || enemySuit != Suit::CLUBS)
        {
            activateClubs(currentPlay.clubsValue);
        }
        else
        {
            addToLog(trText("Clubs power blocked by enemy immunity!",
                            "Poder de Paus bloqueado pela imunidade do inimigo!"));
        }
    }

    // Spades (reduces enemy attack)
    if (currentPlay.hasSpades)
    {
        if (!enemyImmune || enemySuit != Suit::SPADES)
        {
            activateSpades(currentPlay.spadesValue);
        }
        else
        {
            addToLog(trText("Spades power blocked by enemy immunity!",
                            "Poder de Espadas bloqueado pela imunidade do inimigo!"));
        }
    }

    currentPhase = GamePhase::DEAL_DAMAGE;
}

void GameState::activateHearts(int value)
{
    // Shuffle discard pile and put cards under tavern deck
    if (discardPile.isEmpty())
    {
        addToLog(trText("Discard pile is empty - no cards to heal",
                        "Pilha de descarte vazia - nenhuma carta para curar"));
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(discardPile.begin(), discardPile.end(), g);

    int cardsToMove = std::min(value, static_cast<int>(discardPile.size()));
    QVector<Card> cardsToHeal;

    for (int i = 0; i < cardsToMove; i++)
    {
        cardsToHeal.append(discardPile.takeLast());
    }

    for (const Card &card : cardsToHeal)
    {
        tavernDeck.addCardToBottom(card);
    }

    addToLog(trText("Hearts: Moved %1 card(s) from discard to the bottom of the tavern deck",
                    "Copas: Moveu %1 carta(s) do descarte para o fundo do baralho da Taverna")
                 .arg(cardsToMove));
}

void GameState::activateDiamonds(int value)
{
    int cardsDrawn = 0;
    int playerIdx = currentPlayerIndex;

    for (int i = 0; i < value; i++)
    {
        // Find next player who can draw
        int attempts = 0;
        while (attempts < numPlayers)
        {
            Player &player = players[playerIdx];
            if (player.canDrawCard() && !tavernDeck.isEmpty())
            {
                player.addCardToHand(tavernDeck.draw());
                cardsDrawn++;
                break;
            }
            playerIdx = (playerIdx + 1) % numPlayers;
            attempts++;
        }

        if (attempts >= numPlayers)
        {
            break; // No one can draw
        }

        playerIdx = (playerIdx + 1) % numPlayers;
    }

    addToLog(trText("Diamonds: Drew %1 card(s)", "Ouros: Compradas %1 carta(s)").arg(cardsDrawn));
}

void GameState::activateClubs(int value)
{
    // Clubs double damage - handled in dealDamageToEnemy
    addToLog(trText("Clubs: Damage will be doubled (%1)", "Paus: Dano será dobrado (%1)")
                 .arg(value * 2));
}

void GameState::activateSpades(int value)
{
    currentEnemy->addShield(value);
    addToLog(trText("Spades: Reduced enemy attack by %1 (Current shield: %2)",
                    "Espadas: Reduziu o ataque do inimigo em %1 (Escudo atual: %2)")
                 .arg(value)
                 .arg(currentEnemy->getShieldValue()));
}

void GameState::dealDamageToEnemy()
{
    if (currentPhase != GamePhase::DEAL_DAMAGE)
    {
        return;
    }

    int damage = currentPlay.totalAttackValue;

    // Apply clubs double damage
    bool enemyImmune = currentEnemy->getImmunityActive();
    if (currentPlay.hasClubs && (!enemyImmune || currentEnemy->getSuit() != Suit::CLUBS))
    {
        damage = currentPlay.totalAttackValue + currentPlay.clubsValue; // Total damage doubles the clubs portion
    }

    currentEnemy->dealDamage(damage);

    addToLog(trText("Dealt %1 damage to %2", "Causou %1 de dano em %2")
                 .arg(damage)
                 .arg(currentEnemy->toString()));
    addToLog(trText("Enemy HP: %1/%2", "HP do inimigo: %1/%2")
                 .arg(currentEnemy->getCurrentHealth())
                 .arg(currentEnemy->getMaxHealth()));

    // Add played cards to the pile against this enemy
    for (const Card &card : currentPlay.cards)
    {
        playedAgainstEnemy.append(card);
    }

    if (currentEnemy->isDefeated())
    {
        bool exactDamage = (currentEnemy->getDamageDealt() == currentEnemy->getMaxHealth());
        defeatCurrentEnemy(exactDamage);

        if (!castleDeck.isEmpty())
        {
            revealNextEnemy();
            currentPhase = GamePhase::PLAY_CARD;
            // Current player continues
        }
        else
        {
            currentPhase = GamePhase::VICTORY;
            addToLog(trText("VICTORY! All enemies defeated!",
                            "VITÓRIA! Todos os inimigos foram derrotados!"));
        }
    }
    else
    {
        currentPhase = GamePhase::SUFFER_DAMAGE;
    }
}

void GameState::defeatCurrentEnemy(bool exactDamage)
{
    addToLog(trText("Enemy defeated!", "Inimigo derrotado!"));

    Card enemyCard(currentEnemy->getSuit(), currentEnemy->getRank());

    if (exactDamage)
    {
        tavernDeck.addCard(enemyCard);
        addToLog(trText("Perfect damage! Enemy card added to the tavern deck",
                        "Dano perfeito! Carta do inimigo adicionada ao baralho da Taverna"));
    }
    else
    {
        discardPile.append(enemyCard);
        addToLog(trText("Enemy card discarded", "Carta do inimigo descartada"));
    }

    // Discard all cards played against this enemy
    for (const Card &card : playedAgainstEnemy)
    {
        discardPile.append(card);
    }
    playedAgainstEnemy.clear();
    currentPlay.clear();
}

void GameState::revealNextEnemy()
{
    if (!castleDeck.isEmpty())
    {
        Card enemyCard = castleDeck.draw();
        currentEnemy = std::make_unique<Enemy>(enemyCard);
        addToLog(trText("New enemy: %1", "Novo inimigo: %1").arg(currentEnemy->toString()));
        jesterActive = false;
    }
}

bool GameState::sufferDamage(int playerIndex, const QVector<int> &cardIndices)
{
    if (playerIndex != currentPlayerIndex)
    {
        return false;
    }

    if (currentPhase != GamePhase::SUFFER_DAMAGE)
    {
        return false;
    }

    Player &player = players[playerIndex];
    int damageToSuffer = currentEnemy->getEffectiveAttack();

    addToLog(player.getName() + trText(" must discard %1 damage",
                                       " deve descartar %1 de dano")
                                    .arg(damageToSuffer));

    int totalDiscarded = 0;
    QVector<int> sortedIndices = cardIndices;
    std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<int>());

    for (int index : sortedIndices)
    {
        if (index >= 0 && index < player.getHandSize())
        {
            Card card = player.discardCard(index);
            totalDiscarded += card.getValue();
            discardPile.append(card);
            addToLog(trText("  Discarded: %1 (Value: %2)",
                            "  Descartou: %1 (Valor: %2)")
                         .arg(card.toString())
                         .arg(card.getValue()));
        }
    }

    if (totalDiscarded < damageToSuffer)
    {
        addToLog(trText("GAME OVER! %1 couldn't discard enough cards!",
                        "DERROTA! %1 não conseguiu descartar dano suficiente!")
                     .arg(player.getName()));
        currentPhase = GamePhase::GAME_OVER;
        return false;
    }

    addToLog(trText("Total discarded: %1", "Total descartado: %1").arg(totalDiscarded));

    // Move to next player
    nextPlayer();
    currentPhase = GamePhase::PLAY_CARD;
    currentPlay.clear();

    return true;
}

bool GameState::isGameOver() const
{
    return currentPhase == GamePhase::GAME_OVER;
}

bool GameState::isVictory() const
{
    return currentPhase == GamePhase::VICTORY;
}

bool GameState::canYield(int playerIndex) const
{
    // Can't yield if all other players yielded on their last turn
    if (consecutiveYields >= numPlayers - 1)
    {
        return false;
    }
    return true;
}

void GameState::addToLog(const QString &message)
{
    gameLog += message + "\n";
}

void GameState::nextPlayer()
{
    currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
}

bool GameState::isValidCombo(const QVector<Card> &cards) const
{
    if (cards.isEmpty())
    {
        return false;
    }

    // Single card is always valid
    if (cards.size() == 1)
    {
        return true;
    }

    // Check for Animal Companion pairing (max 2 cards, one must be animal)
    bool hasAnimal = false;
    bool hasJester = false;
    for (const Card &card : cards)
    {
        if (card.getIsAnimalCompanion())
            hasAnimal = true;
        if (card.getIsJester())
            hasJester = true;
    }

    // Jester must be played alone
    if (hasJester)
    {
        return cards.size() == 1;
    }

    // Animal Companion can pair with one other card
    if (hasAnimal)
    {
        return cards.size() == 2;
    }

    // Regular combo: same rank, total value <= 10
    Rank firstRank = cards[0].getRank();
    int totalValue = 0;

    for (const Card &card : cards)
    {
        if (card.getRank() != firstRank)
        {
            return false;
        }
        totalValue += card.getValue();
    }

    return totalValue <= 10 && cards.size() <= 4;
}

int GameState::calculateComboAttackValue(const QVector<Card> &cards) const
{
    int total = 0;
    for (const Card &card : cards)
    {
        if (!card.getIsJester())
        {
            total += card.getValue();
        }
    }
    return total;
}

QString GameState::trText(const QString &english, const QString &portuguese) const
{
    return Localization::text(language, english, portuguese);
}
