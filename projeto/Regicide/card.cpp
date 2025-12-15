#include "card.h"

Language Card::languageSetting = Language::Portuguese;

Card::Card(Suit s, Rank r) : suit(s), rank(r), isAnimalCompanion(false), isJester(false) {}

Card::Card(Suit s, bool animal) : suit(s), rank(Rank::ANIMAL_COMPANION),
                                  isAnimalCompanion(animal), isJester(false) {}

Card::Card() : suit(Suit::HEARTS), rank(Rank::JESTER),
               isAnimalCompanion(false), isJester(true) {}

int Card::getValue() const
{
    if (isJester)
        return 0;
    if (isAnimalCompanion)
        return 1;

    // For face cards in hand (defeated enemies)
    if (rank == Rank::JACK)
        return 10;
    if (rank == Rank::QUEEN)
        return 15;
    if (rank == Rank::KING)
        return 20;

    return static_cast<int>(rank);
}

QString Card::toString() const
{
    if (isJester)
        return Localization::text(languageSetting, "Jester", "Bobo");
    if (isAnimalCompanion)
    {
        QString pattern = Localization::text(languageSetting,
                                             "Animal Companion of %1",
                                             "Companheiro Animal de %1");
        return pattern.arg(getSuitString());
    }
    QString pattern = Localization::text(languageSetting,
                                         "%1 of %2",
                                         "%1 de %2");
    return pattern.arg(getRankString(), getSuitString());
}

QString Card::getSuitString() const
{
    switch (suit)
    {
    case Suit::HEARTS:
        return Localization::text(languageSetting, "Hearts", "Copas");
    case Suit::DIAMONDS:
        return Localization::text(languageSetting, "Diamonds", "Ouros");
    case Suit::CLUBS:
        return Localization::text(languageSetting, "Clubs", "Paus");
    case Suit::SPADES:
        return Localization::text(languageSetting, "Spades", "Espadas");
    default:
        return Localization::text(languageSetting, "Unknown", "Desconhecido");
    }
}

QString Card::getRankString() const
{
    if (isJester)
        return Localization::text(languageSetting, "Jester", "Bobo");
    if (isAnimalCompanion)
        return "Animal";

    switch (rank)
    {
    case Rank::TWO:
        return "2";
    case Rank::THREE:
        return "3";
    case Rank::FOUR:
        return "4";
    case Rank::FIVE:
        return "5";
    case Rank::SIX:
        return "6";
    case Rank::SEVEN:
        return "7";
    case Rank::EIGHT:
        return "8";
    case Rank::NINE:
        return "9";
    case Rank::TEN:
        return "10";
    case Rank::JACK:
        return Localization::text(languageSetting, "Jack", "Valete");
    case Rank::QUEEN:
        return Localization::text(languageSetting, "Queen", "Rainha");
    case Rank::KING:
        return Localization::text(languageSetting, "King", "Rei");
    default:
        return Localization::text(languageSetting, "Unknown", "Desconhecido");
    }
}

int Card::getEnemyValue(Rank r)
{
    if (r == Rank::JACK)
        return 10;
    if (r == Rank::QUEEN)
        return 15;
    if (r == Rank::KING)
        return 20;
    return static_cast<int>(r);
}
