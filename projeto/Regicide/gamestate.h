#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "card.h"
#include "deck.h"
#include "player.h"
#include "enemy.h"
#include "language.h"
#include <QVector>
#include <QString>
#include <memory>

enum class GamePhase
{
    SETUP,
    PLAY_CARD,
    ACTIVATE_POWER,
    DEAL_DAMAGE,
    SUFFER_DAMAGE,
    GAME_OVER,
    VICTORY
};

struct PlayedCards
{
    QVector<Card> cards;
    int totalAttackValue;
    bool hasHearts;
    bool hasDiamonds;
    bool hasClubs;
    bool hasSpades;
    int heartsValue;
    int diamondsValue;
    int clubsValue;
    int spadesValue;

    PlayedCards() : totalAttackValue(0), hasHearts(false), hasDiamonds(false),
                    hasClubs(false), hasSpades(false), heartsValue(0),
                    diamondsValue(0), clubsValue(0), spadesValue(0) {}

    void clear()
    {
        cards.clear();
        totalAttackValue = 0;
        hasHearts = hasDiamonds = hasClubs = hasSpades = false;
        heartsValue = diamondsValue = clubsValue = spadesValue = 0;
    }
};

class GameState
{
private:
    int numPlayers;
    QVector<Player> players;
    int currentPlayerIndex;

    Deck tavernDeck;
    Deck castleDeck;
    QVector<Card> discardPile;
    QVector<Card> playedAgainstEnemy;

    std::unique_ptr<Enemy> currentEnemy;

    GamePhase currentPhase;
    PlayedCards currentPlay;

    int consecutiveYields;
    bool jesterActive;
    int jestersUsed; // For solo play

    QString gameLog;
    Language language;

public:
    GameState(int numPlayers, Language language = Language::Portuguese);

    void setupGame();
    void dealInitialHands();
    void revealFirstEnemy();

    // Turn actions
    bool playCards(int playerIndex, const QVector<int> &cardIndices);
    bool yieldTurn(int playerIndex);
    void activateSuitPowers();
    void dealDamageToEnemy();
    bool sufferDamage(int playerIndex, const QVector<int> &cardIndices);

    // Card effects
    void activateHearts(int value);
    void activateDiamonds(int value);
    void activateClubs(int value);
    void activateSpades(int value);

    // Enemy management
    void defeatCurrentEnemy(bool exactDamage);
    void revealNextEnemy();

    // Game state queries
    bool isGameOver() const;
    bool isVictory() const;
    bool canYield(int playerIndex) const;

    // Getters
    const QVector<Player> &getPlayers() const { return players; }
    Player &getCurrentPlayer() { return players[currentPlayerIndex]; }
    const Player &getCurrentPlayer() const { return players[currentPlayerIndex]; }
    const Enemy *getCurrentEnemy() const { return currentEnemy.get(); }
    const Deck &getTavernDeck() const { return tavernDeck; }
    const QVector<Card> &getDiscardPile() const { return discardPile; }
    GamePhase getCurrentPhase() const { return currentPhase; }
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    const QString &getGameLog() const { return gameLog; }
    Language getLanguage() const { return language; }

    void addToLog(const QString &message);
    void nextPlayer();

    // Combo validation
    bool isValidCombo(const QVector<Card> &cards) const;
    int calculateComboAttackValue(const QVector<Card> &cards) const;

private:
    QString trText(const QString &english, const QString &portuguese) const;
};

#endif // GAMESTATE_H
