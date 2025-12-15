#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <QVector>
#include <QString>

class Player
{
private:
    QVector<Card> hand;
    int maxHandSize;
    QString name;
    int playerId;
    bool hasYielded;
    Language language;

public:
    Player(int id, int maxHand, Language language);

    void addCardToHand(const Card &card);
    Card playCard(int index);
    Card discardCard(int index);

    int getHandSize() const { return hand.size(); }
    int getMaxHandSize() const { return maxHandSize; }
    const QVector<Card> &getHand() const { return hand; }
    QString getName() const { return name; }
    void setName(const QString &n) { name = n; }
    int getPlayerId() const { return playerId; }

    bool canDrawCard() const { return hand.size() < maxHandSize; }
    bool hasCards() const { return !hand.isEmpty(); }

    void setYielded(bool yielded) { hasYielded = yielded; }
    bool getYielded() const { return hasYielded; }
    Language getLanguage() const { return language; }

    // Calculate total value of cards in hand for damage coverage
    int getTotalHandValue() const;
};

#endif // PLAYER_H
