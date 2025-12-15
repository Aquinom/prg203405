#include "deck.h"
#include <QRandomGenerator>

Deck::Deck() {}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::draw()
{
    if (cards.isEmpty())
    {
        throw std::runtime_error("Cannot draw from empty deck");
    }
    Card card = cards.last();
    cards.removeLast();
    return card;
}

void Deck::addCard(const Card &card)
{
    cards.append(card);
}

void Deck::addCardToBottom(const Card &card)
{
    cards.prepend(card);
}

Deck Deck::createTavernDeck(int numPlayers)
{
    Deck deck;

    // Add numbered cards (2-10) for all suits
    for (int suit = 0; suit < 4; suit++)
    {
        for (int rank = 2; rank <= 10; rank++)
        {
            deck.addCard(Card(static_cast<Suit>(suit), static_cast<Rank>(rank)));
        }
    }

    // Add 4 Animal Companions
    deck.addCard(Card(Suit::HEARTS, true));
    deck.addCard(Card(Suit::DIAMONDS, true));
    deck.addCard(Card(Suit::CLUBS, true));
    deck.addCard(Card(Suit::SPADES, true));

    // Add Jesters based on number of players
    int numJesters = 0;
    if (numPlayers == 3)
        numJesters = 1;
    else if (numPlayers == 4)
        numJesters = 2;

    for (int i = 0; i < numJesters; i++)
    {
        deck.addCard(Card()); // Jester
    }

    deck.shuffle();
    return deck;
}

Deck Deck::createCastleDeck()
{
    Deck deck;

    // Add Jacks (bottom layer)
    QVector<Card> jacks;
    for (int suit = 0; suit < 4; suit++)
    {
        jacks.append(Card(static_cast<Suit>(suit), Rank::JACK));
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(jacks.begin(), jacks.end(), g);
    for (const Card &jack : jacks)
    {
        deck.addCard(jack);
    }

    // Add Queens (middle layer)
    QVector<Card> queens;
    for (int suit = 0; suit < 4; suit++)
    {
        queens.append(Card(static_cast<Suit>(suit), Rank::QUEEN));
    }
    std::shuffle(queens.begin(), queens.end(), g);
    for (const Card &queen : queens)
    {
        deck.addCard(queen);
    }

    // Add Kings (top layer)
    QVector<Card> kings;
    for (int suit = 0; suit < 4; suit++)
    {
        kings.append(Card(static_cast<Suit>(suit), Rank::KING));
    }
    std::shuffle(kings.begin(), kings.end(), g);
    for (const Card &king : kings)
    {
        deck.addCard(king);
    }

    return deck;
}
