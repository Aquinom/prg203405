#include "enemy.h"

Enemy::Enemy(Card card) : suit(card.getSuit()), rank(card.getRank()),
                          damageDealt(0), shieldValue(0), immunityActive(true)
{
    initializeStats();
}

void Enemy::initializeStats()
{
    switch (rank)
    {
    case Rank::JACK:
        attackValue = 10;
        maxHealth = 20;
        currentHealth = 20;
        break;
    case Rank::QUEEN:
        attackValue = 15;
        maxHealth = 30;
        currentHealth = 30;
        break;
    case Rank::KING:
        attackValue = 20;
        maxHealth = 40;
        currentHealth = 40;
        break;
    default:
        attackValue = 0;
        maxHealth = 0;
        currentHealth = 0;
        break;
    }
}

void Enemy::dealDamage(int damage)
{
    damageDealt += damage;
    currentHealth = maxHealth - damageDealt;
    if (currentHealth < 0)
        currentHealth = 0;
}

void Enemy::addShield(int shield)
{
    shieldValue += shield;
}

void Enemy::resetForNewEnemy(Card card)
{
    suit = card.getSuit();
    rank = card.getRank();
    damageDealt = 0;
    shieldValue = 0;
    immunityActive = true;
    initializeStats();
}

int Enemy::getEffectiveAttack() const
{
    int effective = attackValue - shieldValue;
    return effective > 0 ? effective : 0;
}

QString Enemy::toString() const
{
    Card card(suit, rank);
    return card.toString();
}
