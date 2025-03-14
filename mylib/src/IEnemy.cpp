#include "IEnemy.h"
#include <cmath>
#include <iostream>
#include <random>

IEnemy::IEnemy(const std::string& name)
    : CompositeGameObject(name)
    , m_health(100)
    , m_maxHealth(100)
    , m_damage(10)
    , m_speed(100.f)
    , m_detectionRadius(600.f)
    , m_attackRadius(50.f)
    , m_isPlayerDetected(false)
    , m_playerPos(0.f, 0.f)
    , m_lastKnownPlayerPos(0.f, 0.f)
    , m_healthIsCritical(0.3f)
    , m_retreat(0.25f)
    , m_currentPatrolPoint(0)
    , m_currentState(nullptr)
{
    setCategory("Enemy");

    float patrolRadius = 10.f;
    m_patrolPoints.push_back(sf::Vector2f(-patrolRadius, -patrolRadius));
    m_patrolPoints.push_back(sf::Vector2f(patrolRadius, -patrolRadius));
    m_patrolPoints.push_back(sf::Vector2f(patrolRadius, patrolRadius));
    m_patrolPoints.push_back(sf::Vector2f(-patrolRadius, patrolRadius));
}

IEnemy::~IEnemy()
{
    delete m_currentState;
}

void IEnemy::init(const sf::Vector2f& position, const float& size,
    const sf::Color& color, const float& speed)
{
    auto renderer = std::make_shared<SquareRenderer>(size, color);
    addComponent(renderer);
    renderer->setPosition(position);

    m_speed = speed;
}

void IEnemy::update(const float& deltaTime)
{
    if (m_currentState)
        m_currentState->update(this, deltaTime);

    CompositeGameObject::update(deltaTime);
}

void IEnemy::changeState(IEnemyState* newState)
{
    if (m_currentState)
    {
        m_currentState->exit(this);
        delete m_currentState;
    }

    m_currentState = newState;
    if (m_currentState)
        m_currentState->enter(this);
}

bool IEnemy::isHealthCritical() const
{
    return static_cast<float>(m_health) / m_maxHealth <= m_healthIsCritical;
}

bool IEnemy::canDetectPlayer() const
{
    return m_isPlayerDetected;
}

bool IEnemy::isWithinAttackRange() const
{
    auto render = static_cast<const SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render || !m_isPlayerDetected)
        return false;

    sf::Vector2f pos = render->getPosition();
    float distance = (pos.x - m_playerPos.x) * (pos.x - m_playerPos.x) +
        (pos.y - m_playerPos.y) * (pos.y - m_playerPos.y);

    return distance <= m_attackRadius * m_attackRadius;
}

bool IEnemy::shouldRetreat() const
{
    return static_cast<float>(m_health) / m_maxHealth <= m_retreat;
}

void IEnemy::patrol(float deltaTime)
{
    if (m_patrolPoints.empty())
        return;

    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render)
        return;

    sf::Vector2f currentPos = render->getPosition();
    sf::Vector2f targetPos = m_patrolPoints[m_currentPatrolPoint];

    targetPos.x += currentPos.x;
    targetPos.y += currentPos.y;

    sf::Vector2f direction = targetPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 10.0f)
    {
        m_currentPatrolPoint = (m_currentPatrolPoint + 1) % m_patrolPoints.size();
        return;
    }

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * deltaTime;
        render->setPosition(currentPos + direction);
    }
}

void IEnemy::moveTowardPlayer(float deltaTime)
{
    if (!m_isPlayerDetected)
        return;

    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render)
        return;

    sf::Vector2f currentPos = render->getPosition();
    sf::Vector2f direction = m_playerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= m_attackRadius * 0.8f)
        return;

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * deltaTime;
        render->setPosition(currentPos + direction);
    }
}

void IEnemy::retreatFromPlayer(float deltaTime)
{
    if (!m_isPlayerDetected)
        return;

    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render)
        return;

    sf::Vector2f currentPos = render->getPosition();
    sf::Vector2f direction = currentPos - m_playerPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * 1.2f * deltaTime;
        render->setPosition(currentPos + direction);
    }
}

void IEnemy::searchingArea(float deltaTime)
{
    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render)
        return;

    sf::Vector2f currentPos = render->getPosition();
    sf::Vector2f direction = m_lastKnownPlayerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 20.0f)
    {
        patrol(deltaTime);
        return;
    }

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * deltaTime;
        render->setPosition(currentPos + direction);
    }
}

void IEnemy::takeDamage(int amount)
{
    m_health = std::max(0, m_health - amount);

    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (render)
        updateLOS(m_playerPos);
}

void IEnemy::forgetPlayer()
{
    m_isPlayerDetected = false;
}

void IEnemy::updateLOS(const sf::Vector2f& playerPos)
{
    auto render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!render)
        return;

    sf::Vector2f currentPos = render->getPosition();
    float distSq = (currentPos.x - playerPos.x) * (currentPos.x - playerPos.x) +
        (currentPos.y - playerPos.y) * (currentPos.y - playerPos.y);

    bool wasDetected = m_isPlayerDetected;
    m_isPlayerDetected = distSq <= m_detectionRadius * m_detectionRadius;

    if (m_isPlayerDetected)
    {
        m_playerPos = playerPos;
        m_lastKnownPlayerPos = playerPos;

        if (!wasDetected)
            std::cout << getName() << " spotted the player!" << std::endl;
    }
    else if (wasDetected)
        std::cout << getName() << " lost sight of the player!" << std::endl;
}

void IEnemy::updateAllEnemyLOS(std::vector<std::shared_ptr<CompositeGameObject>>& gameObjects,
    const sf::Vector2f& playerPos)
{
    for (auto& obj : gameObjects)
    {
        if (obj->getCategory() == "Enemy")
        {
            auto enemy = dynamic_cast<IEnemy*>(obj.get());
            if (enemy)
                enemy->updateLOS(playerPos);
        }
    }
}