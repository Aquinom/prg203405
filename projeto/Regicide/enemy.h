#ifndef ENEMY_H
#define ENEMY_H

#include "card.h"
#include <QString>

class Enemy
{
private:
    Suit suit;
    Rank rank;
    int maxHealth;
    int currentHealth;
    int attackValue;
    int damageDealt;
    int shieldValue; // Total spades shield
    bool immunityActive;

public:
    Enemy(Card card);

    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    int getMaxHealth() const { return maxHealth; }
    int getCurrentHealth() const { return currentHealth; }
    int getAttackValue() const { return attackValue; }
    int getDamageDealt() const { return damageDealt; }
    int getShieldValue() const { return shieldValue; }
    bool getImmunityActive() const { return immunityActive; }

    void dealDamage(int damage);
    void addShield(int shield);
    void cancelImmunity() { immunityActive = false; }
    void resetForNewEnemy(Card card);

    int getEffectiveAttack() const;
    bool isDefeated() const { return damageDealt >= maxHealth; }

    QString toString() const;

private:
    void initializeStats();
};

#endif // ENEMY_H
