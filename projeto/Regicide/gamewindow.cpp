#include "gamewindow.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFont>
#include <QScrollBar>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent), numPlayers(1), currentLanguage(Language::Portuguese)
{
    setWindowTitle(text("Regicide - Cooperative Card Game", "Regicida - Jogo de Cartas Cooperativo"));
    resize(900, 700);

    mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    setupUI();
    showStartScreen();
}

void GameWindow::setupUI()
{
    // Start screen will be created in showStartScreen()
    startWidget = nullptr;
    gameWidget = nullptr;
    languageCombo = nullptr;
}

void GameWindow::showStartScreen()
{
    // Clear main layout
    QLayoutItem *item;
    while ((item = mainLayout->takeAt(0)) != nullptr)
    {
        if (item->widget())
        {
            item->widget()->deleteLater();
        }
        delete item;
    }

    startWidget = new QWidget(this);
    QVBoxLayout *startLayout = new QVBoxLayout(startWidget);
    startLayout->setAlignment(Qt::AlignCenter);
    startLayout->setSpacing(20);

    // Title
    QLabel *titleLabel = new QLabel(text("REGICIDE", "REGICIDA"), startWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(36);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    QLabel *subtitleLabel = new QLabel(startWidget);
    QFont subtitleFont = subtitleLabel->font();
    subtitleFont.setPointSize(14);
    subtitleFont.setItalic(true);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Description
    QLabel *descLabel = new QLabel(startWidget);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);

    // Player count selection
    QLabel *playerLabel = new QLabel(startWidget);
    playerLabel->setAlignment(Qt::AlignCenter);

    // Language selector
    QHBoxLayout *languageLayout = new QHBoxLayout();
    QLabel *languageLabel = new QLabel(startWidget);
    languageCombo = new QComboBox(startWidget);
    languageCombo->addItem("Português");
    languageCombo->addItem("English");
    languageCombo->setCurrentIndex(currentLanguage == Language::Portuguese ? 0 : 1);

    auto updateStartTexts = [this, subtitleLabel, descLabel, playerLabel, languageLabel]()
    {
        setWindowTitle(text("Regicide - Cooperative Card Game", "Regicida - Jogo de Cartas Cooperativo"));
        subtitleLabel->setText(text("Cooperative Card Game", "Jogo de Cartas Cooperativo"));
        descLabel->setText(text(
            "Work together to defeat 12 powerful enemies:\n4 Jacks, 4 Queens, and 4 Kings.\n\nUse card powers wisely to survive!",
            "Unam forças para derrotar 12 inimigos poderosos:\n4 Valetes, 4 Rainhas e 4 Reis.\n\nUse os poderes das cartas com sabedoria para sobreviver!"));
        playerLabel->setText(text("Select number of players:", "Selecione o número de jogadores:"));
        languageLabel->setText(text("Language:", "Idioma:"));
    };

    updateStartTexts();

    connect(languageCombo, qOverload<int>(&QComboBox::currentIndexChanged), this,
            [this, updateStartTexts](int index) mutable
            {
                currentLanguage = (index == 0) ? Language::Portuguese : Language::English;
                updateStartTexts();
            });

    languageLayout->addStretch();
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageCombo);
    languageLayout->addStretch();

    QHBoxLayout *playerButtonLayout = new QHBoxLayout();
    playerButtonLayout->setAlignment(Qt::AlignCenter);
    playerButtonLayout->setSpacing(10);

    for (int i = 1; i <= 4; i++)
    {
        QPushButton *playerBtn = new QPushButton(QString::number(i), startWidget);
        playerBtn->setFixedSize(60, 60);
        playerBtn->setProperty("playerCount", i);
        connect(playerBtn, &QPushButton::clicked, this, [this, i]()
                {
            numPlayers = i;
            startNewGame(); });
        playerButtonLayout->addWidget(playerBtn);
    }

    startLayout->addStretch();
    startLayout->addWidget(titleLabel);
    startLayout->addLayout(languageLayout);
    startLayout->addWidget(subtitleLabel);
    startLayout->addSpacing(30);
    startLayout->addWidget(descLabel);
    startLayout->addSpacing(30);
    startLayout->addWidget(playerLabel);
    startLayout->addLayout(playerButtonLayout);
    startLayout->addStretch();

    mainLayout->addWidget(startWidget);
}

void GameWindow::startNewGame()
{
    if (languageCombo)
    {
        currentLanguage = (languageCombo->currentIndex() == 0) ? Language::Portuguese : Language::English;
    }

    Card::setLanguage(currentLanguage);
    gameState = std::make_unique<GameState>(numPlayers, currentLanguage);
    try
    {
        gameState->setupGame();
        showGameScreen();
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this,
                              text("Error", "Erro"),
                              text("Failed to start game: %1", "Falha ao iniciar o jogo: %1").arg(e.what()));
    }
}

void GameWindow::showGameScreen()
{
    // Clear main layout
    if (startWidget)
    {
        startWidget->deleteLater();
        startWidget = nullptr;
    }

    gameWidget = new QWidget(this);
    QVBoxLayout *gameLayout = new QVBoxLayout(gameWidget);

    // Top section: Enemy info
    QWidget *enemyWidget = new QWidget(gameWidget);
    QVBoxLayout *enemyLayout = new QVBoxLayout(enemyWidget);
    enemyLayout->setContentsMargins(10, 10, 10, 10);
    enemyWidget->setStyleSheet("background-color: #2C3E50; border-radius: 5px;");

    enemyLabel = new QLabel(text("Enemy:", "Inimigo:"), enemyWidget);
    QFont enemyFont = enemyLabel->font();
    enemyFont.setPointSize(16);
    enemyFont.setBold(true);
    enemyLabel->setFont(enemyFont);
    enemyLabel->setStyleSheet("color: white;");

    enemyStatsLabel = new QLabel("", enemyWidget);
    enemyStatsLabel->setStyleSheet("color: white;");
    QFont statsFont = enemyStatsLabel->font();
    statsFont.setPointSize(12);
    enemyStatsLabel->setFont(statsFont);

    enemyLayout->addWidget(enemyLabel);
    enemyLayout->addWidget(enemyStatsLabel);

    // Phase and player info
    QHBoxLayout *infoLayout = new QHBoxLayout();

    phaseLabel = new QLabel(text("Phase: Play Card", "Fase: Jogar Carta"), gameWidget);
    currentPlayerLabel = new QLabel(text("Current Player:", "Jogador Atual:"), gameWidget);
    deckInfoLabel = new QLabel(text("Tavern Deck:", "Baralho da Taverna:"), gameWidget);

    infoLayout->addWidget(currentPlayerLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(phaseLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(deckInfoLabel);

    // Player hand
    QLabel *handLabel = new QLabel(text("Your Hand:", "Sua Mão:"), gameWidget);
    QFont handFont = handLabel->font();
    handFont.setPointSize(12);
    handFont.setBold(true);
    handLabel->setFont(handFont);

    handListWidget = new QListWidget(gameWidget);
    handListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    handListWidget->setMaximumHeight(150);

    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    playButton = new QPushButton(text("Play Selected Cards", "Jogar Cartas Selecionadas"), gameWidget);
    yieldButton = new QPushButton(text("Yield Turn", "Ceder Turno"), gameWidget);
    confirmButton = new QPushButton(text("Confirm Discards", "Confirmar Descartes"), gameWidget);

    playButton->setMinimumHeight(40);
    yieldButton->setMinimumHeight(40);
    confirmButton->setMinimumHeight(40);

    connect(playButton, &QPushButton::clicked, this, &GameWindow::playSelectedCards);
    connect(yieldButton, &QPushButton::clicked, this, &GameWindow::yieldTurn);
    connect(confirmButton, &QPushButton::clicked, this, &GameWindow::confirmDiscards);

    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(yieldButton);
    buttonLayout->addWidget(confirmButton);

    // Game log
    QLabel *logLabel = new QLabel(text("Game Log:", "Registro do Jogo:"), gameWidget);
    logTextEdit = new QTextEdit(gameWidget);
    logTextEdit->setReadOnly(true);
    logTextEdit->setMaximumHeight(200);

    // Add all to game layout
    gameLayout->addWidget(enemyWidget);
    gameLayout->addSpacing(10);
    gameLayout->addLayout(infoLayout);
    gameLayout->addSpacing(10);
    gameLayout->addWidget(handLabel);
    gameLayout->addWidget(handListWidget);
    gameLayout->addSpacing(10);
    gameLayout->addLayout(buttonLayout);
    gameLayout->addSpacing(10);
    gameLayout->addWidget(logLabel);
    gameLayout->addWidget(logTextEdit);

    mainLayout->addWidget(gameWidget);

    updateGameDisplay();
}

void GameWindow::updateGameDisplay()
{
    if (!gameState)
        return;

    updateEnemyDisplay();
    updatePlayerHand();
    updateGameLog();
    processGamePhase();
}

void GameWindow::updateEnemyDisplay()
{
    const Enemy *enemy = gameState->getCurrentEnemy();
    if (enemy)
    {
        enemyLabel->setText(text("Current Enemy: %1", "Inimigo Atual: %1").arg(enemy->toString()));

        QString stats = text("HP: %1/%2 | Attack: %3 | Shield: %4",
                             "HP: %1/%2 | Ataque: %3 | Escudo: %4")
                            .arg(enemy->getCurrentHealth())
                            .arg(enemy->getMaxHealth())
                            .arg(enemy->getAttackValue())
                            .arg(enemy->getShieldValue());

        if (enemy->getEffectiveAttack() != enemy->getAttackValue())
        {
            stats += " | " + text("Effective Attack: %1", "Ataque Efetivo: %1")
                                 .arg(enemy->getEffectiveAttack());
        }

        if (enemy->getImmunityActive())
        {
            stats += " | " + text("IMMUNE to %1", "IMUNE a %1").arg(suitName(enemy->getSuit()));
        }

        enemyStatsLabel->setText(stats);
    }
}

void GameWindow::updatePlayerHand()
{
    handListWidget->clear();

    const Player &currentPlayer = gameState->getCurrentPlayer();
    const QVector<Card> &hand = currentPlayer.getHand();

    currentPlayerLabel->setText(
        text("Current Player: %1 (Hand: %2/%3)", "Jogador Atual: %1 (Mão: %2/%3)")
            .arg(currentPlayer.getName())
            .arg(hand.size())
            .arg(currentPlayer.getMaxHandSize()));

    for (int i = 0; i < hand.size(); i++)
    {
        QString cardText = text("%1. %2 (Value: %3)", "%1. %2 (Valor: %3)")
                               .arg(i + 1)
                               .arg(hand[i].toString())
                               .arg(hand[i].getValue());
        handListWidget->addItem(cardText);
    }

    deckInfoLabel->setText(
        text("Tavern: %1 | Discard: %2", "Taverna: %1 | Descarte: %2")
            .arg(gameState->getTavernDeck().size())
            .arg(gameState->getDiscardPile().size()));
}

void GameWindow::updateGameLog()
{
    logTextEdit->setPlainText(gameState->getGameLog());
    logTextEdit->verticalScrollBar()->setValue(logTextEdit->verticalScrollBar()->maximum());
}

void GameWindow::processGamePhase()
{
    GamePhase phase = gameState->getCurrentPhase();

    playButton->setEnabled(false);
    yieldButton->setEnabled(false);
    confirmButton->setEnabled(false);

    switch (phase)
    {
    case GamePhase::PLAY_CARD:
        phaseLabel->setText(text("Phase: Play Card or Yield", "Fase: Jogar Carta ou Ceder"));
        playButton->setEnabled(true);
        yieldButton->setEnabled(gameState->canYield(gameState->getCurrentPlayerIndex()));
        break;

    case GamePhase::ACTIVATE_POWER:
        phaseLabel->setText(text("Phase: Activating Powers...", "Fase: Ativando Poderes..."));
        gameState->activateSuitPowers();
        updateGameDisplay();
        break;

    case GamePhase::DEAL_DAMAGE:
        phaseLabel->setText(text("Phase: Dealing Damage...", "Fase: Causando Dano..."));
        gameState->dealDamageToEnemy();
        updateGameDisplay();
        break;

    case GamePhase::SUFFER_DAMAGE:
        phaseLabel->setText(text("Phase: Discard Cards for Damage", "Fase: Descarte para Reduzir Dano"));
        confirmButton->setEnabled(true);
        break;

    case GamePhase::VICTORY:
        phaseLabel->setText(text("VICTORY!", "VITÓRIA!"));
        QMessageBox::information(this,
                                 text("Victory!", "Vitória!"),
                                 text("Congratulations! You have defeated all enemies!\n\n%1",
                                      "Parabéns! Vocês derrotaram todos os inimigos!\n\n%1")
                                     .arg(gameState->getGameLog()));
        showStartScreen();
        break;

    case GamePhase::GAME_OVER:
        phaseLabel->setText(text("GAME OVER", "DERROTA"));
        QMessageBox::information(this,
                                 text("Game Over", "Derrota"),
                                 text("The party has been defeated!\n\n%1",
                                      "O grupo foi derrotado!\n\n%1")
                                     .arg(gameState->getGameLog()));
        showStartScreen();
        break;

    default:
        break;
    }
}

void GameWindow::playSelectedCards()
{
    clearSelectedCards();

    QList<QListWidgetItem *> selected = handListWidget->selectedItems();

    if (selected.isEmpty())
    {
        QMessageBox::warning(this,
                             text("No Cards", "Nenhuma Carta"),
                             text("Please select at least one card to play.",
                                  "Selecione ao menos uma carta para jogar."));
        return;
    }

    // Get selected indices
    for (QListWidgetItem *item : selected)
    {
        int row = handListWidget->row(item);
        selectedCardIndices.append(row);
    }

    bool success = gameState->playCards(gameState->getCurrentPlayerIndex(), selectedCardIndices);

    if (!success)
    {
        QMessageBox::warning(
            this,
            text("Invalid Play", "Jogada Inválida"),
            text("Invalid card combination. Check combo rules:\n"
                 "- Jester must be played alone\n"
                 "- Animal Companion can pair with one other card\n"
                 "- Regular combos: same rank, total value ≤ 10",
                 "Combinação inválida. Revise as regras:\n"
                 "- O Bobo deve ser jogado sozinho\n"
                 "- Companheiros Animais só podem acompanhar uma carta\n"
                 "- Combos comuns: mesmo valor e total até 10"));
        clearSelectedCards();
        return;
    }

    clearSelectedCards();
    updateGameDisplay();
}

void GameWindow::yieldTurn()
{
    bool success = gameState->yieldTurn(gameState->getCurrentPlayerIndex());

    if (!success)
    {
        QMessageBox::warning(this,
                             text("Cannot Yield", "Não é possível ceder"),
                             text("You cannot yield when all other players have yielded.",
                                  "Você não pode ceder se todos os outros jogadores já cederam."));
        return;
    }

    updateGameDisplay();
}

void GameWindow::confirmDiscards()
{
    clearSelectedCards();

    QList<QListWidgetItem *> selected = handListWidget->selectedItems();

    // Get selected indices
    for (QListWidgetItem *item : selected)
    {
        int row = handListWidget->row(item);
        selectedCardIndices.append(row);
    }

    bool success = gameState->sufferDamage(gameState->getCurrentPlayerIndex(), selectedCardIndices);

    clearSelectedCards();
    updateGameDisplay();

    if (!success && gameState->isGameOver())
    {
        // Game over message already shown in processGamePhase
        return;
    }
}

void GameWindow::clearSelectedCards()
{
    selectedCardIndices.clear();
}

QString GameWindow::text(const QString &english, const QString &portuguese) const
{
    return (currentLanguage == Language::English) ? english : portuguese;
}

QString GameWindow::suitName(Suit suit) const
{
    switch (suit)
    {
    case Suit::HEARTS:
        return text("Hearts", "Copas");
    case Suit::DIAMONDS:
        return text("Diamonds", "Ouros");
    case Suit::CLUBS:
        return text("Clubs", "Paus");
    case Suit::SPADES:
        return text("Spades", "Espadas");
    default:
        return "";
    }
}
