#include "player.h"

Player::Player(int id, int maxHand, Language lang)
    : playerId(id), maxHandSize(maxHand), hasYielded(false), language(lang)
{
    name = Localization::text(language, "Player ", "Jogador ") + QString::number(id + 1);
}

void Player::addCardToHand(const Card &card)
{
    if (hand.size() < maxHandSize)
    {
        hand.append(card);
    }
}

Card Player::playCard(int index)
{
    if (index < 0 || index >= hand.size())
    {
        const QString message = Localization::text(language, "Invalid card index", "Índice de carta inválido");
        throw std::out_of_range(message.toStdString());
    }
    Card card = hand[index];
    hand.removeAt(index);
    return card;
}

Card Player::discardCard(int index)
{
    if (index < 0 || index >= hand.size())
    {
        const QString message = Localization::text(language, "Invalid card index", "Índice de carta inválido");
        throw std::out_of_range(message.toStdString());
    }
    Card card = hand[index];
    hand.removeAt(index);
    return card;
}

int Player::getTotalHandValue() const
{
    int total = 0;
    for (const Card &card : hand)
    {
        total += card.getValue();
    }
    return total;
}
