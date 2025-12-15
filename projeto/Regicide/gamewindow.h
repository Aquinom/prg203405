#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QComboBox>
#include <memory>
#include "gamestate.h"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

private slots:
    void startNewGame();
    void playSelectedCards();
    void yieldTurn();
    void confirmDiscards();

private:
    void setupUI();
    void showStartScreen();
    void showGameScreen();
    void updateGameDisplay();
    void updatePlayerHand();
    void updateEnemyDisplay();
    void updateGameLog();
    void clearSelectedCards();
    void processGamePhase();

    QVBoxLayout *mainLayout;
    QWidget *startWidget;
    QWidget *gameWidget;
    QComboBox *languageCombo;

    // Game screen widgets
    QLabel *enemyLabel;
    QLabel *enemyStatsLabel;
    QLabel *phaseLabel;
    QLabel *currentPlayerLabel;
    QListWidget *handListWidget;
    QPushButton *playButton;
    QPushButton *yieldButton;
    QPushButton *confirmButton;
    QTextEdit *logTextEdit;
    QLabel *deckInfoLabel;

    std::unique_ptr<GameState> gameState;
    QVector<int> selectedCardIndices;

    int numPlayers;
    Language currentLanguage;

    QString text(const QString &english, const QString &portuguese) const;
    QString suitName(Suit suit) const;
};

#endif // GAMEWINDOW_H
