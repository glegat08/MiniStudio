#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <random>

// MeleeEnemy
MeleeEnemy::MeleeEnemy(const std::string& name)
    : IEnemy(name)
    , m_attackCD(1.0f)
    , m_currentAttackCD(0.0f)
    , m_isAttacking(false)
{
    addTag("MeleeEnemy");
    m_damage = 15;
    m_attackRadius = 60.0f;
    m_speed = 100.0f;
}

MeleeEnemy::~MeleeEnemy()
{
}

void MeleeEnemy::init(const sf::Vector2f& pos)
{
    IEnemy::init(pos, 40.0f, sf::Color(200, 50, 50), m_speed);

    changeState(new MeleeEnemyStates::PatrolState());
}

void MeleeEnemy::Attack(float deltaTime)
{
    if (m_currentAttackCD > 0)
    {
        m_currentAttackCD -= deltaTime;
        return;
    }

    if (isWithinAttackRange())
    {
        std::cout << getName() << " performs a melee attack!" << std::endl;
        m_isAttacking = true;

        m_currentAttackCD = m_attackCD;

        auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (render)
            render->setColor(sf::Color::Yellow);
    }

    m_isAttacking = false;
}

void MeleeEnemy::Block(float deltaTime)
{
    std::cout << getName() << " is blocking!" << std::endl;
}

bool MeleeEnemy::canCounterAttack() const
{
    return false;
}

// RangedEnemy
RangedEnemy::RangedEnemy(const std::string& name)
    : IEnemy(name)
    , m_optimalDistance(200.0f)
    , m_fireCD(2.0f)
    , m_currentFireCD(0.0f)
{
    addTag("RangedEnemy");
    m_damage = 10;
    m_attackRadius = 300.0f;
    m_speed = 100.0f;
}

RangedEnemy::~RangedEnemy()
{
}

void RangedEnemy::init(const sf::Vector2f& pos)
{
    IEnemy::init(pos, 35.0f, sf::Color(50, 150, 200), m_speed);

    changeState(new RangedEnemyStates::PatrolState());
}

void RangedEnemy::Attack(float deltaTime)
{
    if (m_currentFireCD > 0)
    {
        m_currentFireCD -= deltaTime;
        return;
    }

    if (isWithinAttackRange() && isAtOptimalDistance())
    {
        std::cout << getName() << " shooting the player!" << std::endl;

        m_currentFireCD = m_fireCD;

        auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (render)
            render->setColor(sf::Color::Cyan);
    }
}

void RangedEnemy::Block(float deltaTime)
{
    std::cout << getName() << " is blocking!" << std::endl;
}

bool RangedEnemy::isAtOptimalDistance() const
{
    if (!canDetectPlayer())
        return false;

    auto renderer = static_cast<const SquareRenderer*>(getComponent("SquareRenderer"));
    if (!renderer)
        return false;

    sf::Vector2f pos = renderer->getPosition();
    float distanceSq = (pos.x - m_playerPos.x) * (pos.x - m_playerPos.x) +
        (pos.y - m_playerPos.y) * (pos.y - m_playerPos.y);

    float distance = std::sqrt(distanceSq);
    float tolerance = 0.2f * m_optimalDistance;

    return std::abs(distance - m_optimalDistance) <= tolerance;
}

void RangedEnemy::maintainDistance(float deltaTime)
{
    if (!canDetectPlayer())
        return;

    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!renderer)
        return;

    sf::Vector2f currentPos = renderer->getPosition();
    sf::Vector2f direction = m_playerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0)
        return;

    direction /= distance;

    if (distance < m_optimalDistance * 0.8f)
        renderer->setPosition(currentPos - direction * m_speed * deltaTime);
    else if (distance > m_optimalDistance * 1.2f)
        renderer->setPosition(currentPos + direction * m_speed * deltaTime);
}