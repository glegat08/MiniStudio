#pragma once
#include "IEnemy.h"

class MeleeEnemy : public IEnemy
{
public:
    MeleeEnemy(const std::string& name = "MeleeEnemy");
    ~MeleeEnemy();

    void init(const sf::Vector2f& pos);

    void Attack(float deltaTime) override;
    void Block(float deltaTime) override;
    bool canCounterAttack() const;

private:
    float m_attackCD;
    float m_currentAttackCD;
    bool m_isAttacking;
};

class RangedEnemy : public IEnemy
{
public:
    RangedEnemy(const std::string& name = "RangedEnemy");
    ~RangedEnemy();

    void init(const sf::Vector2f& pos);

    void Attack(float deltaTime) override;
    void Block(float deltaTime) override;
    bool isAtOptimalDistance() const;
    void maintainDistance(float deltaTime);

private:
    float m_optimalDistance;
    float m_fireCD;
    float m_currentFireCD;
};