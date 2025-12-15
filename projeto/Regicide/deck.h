#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <QVector>
#include <algorithm>
#include <random>

class Deck
{
private:
    QVector<Card> cards;

public:
    Deck();

    void shuffle();
    Card draw();
    void addCard(const Card &card);
    void addCardToBottom(const Card &card);
    int size() const { return cards.size(); }
    bool isEmpty() const { return cards.isEmpty(); }
    void clear() { cards.clear(); }

    QVector<Card> &getCards() { return cards; }
    const QVector<Card> &getCards() const { return cards; }

    // Factory methods
    static Deck createTavernDeck(int numPlayers);
    static Deck createCastleDeck();
};

#endif // DECK_H
