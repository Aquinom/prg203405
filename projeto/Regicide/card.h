#ifndef CARD_H
#define CARD_H

#include <QString>
#include "language.h"

enum class Suit
{
    HEARTS,   // Heal from discard
    DIAMONDS, // Draw cards
    CLUBS,    // Double damage
    SPADES    // Shield against enemy
};

enum class Rank
{
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    ANIMAL_COMPANION = 1,
    JESTER = 0
};

class Card
{
private:
    Suit suit;
    Rank rank;
    bool isAnimalCompanion;
    bool isJester;
    static Language languageSetting;

public:
    Card(Suit s, Rank r);
    Card(Suit s, bool animal); // Animal Companion constructor
    Card();                    // Jester constructor

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    int getValue() const;
    bool getIsAnimalCompanion() const { return isAnimalCompanion; }
    bool getIsJester() const { return isJester; }

    QString toString() const;
    QString getSuitString() const;
    QString getRankString() const;

    static void setLanguage(Language lang) { languageSetting = lang; }
    static Language getLanguage() { return languageSetting; }

    // For defeated enemies (Jacks, Queens, Kings) in hand
    static int getEnemyValue(Rank r);
};

#endif // CARD_H
