#include "IEnemy.h"
#include <cmath>
#include <iostream>
#include <random>

#include "Animation.h"
#include "Collision.h"

IEnemy::IEnemy(const std::string& name)
    : CompositeGameObject(name)
    , m_health(100)
    , m_maxHealth(100)
    , m_damage(10)
    , m_speed(100.f)
    , m_detectionRadius(300.f)
    , m_attackRadius(50.f)
    , m_isPlayerDetected(false)
    , m_playerPos(0.f, 0.f)
    , m_lastKnownPlayerPos(0.f, 0.f)
    , m_healthIsCritical(0.3f)
    , m_retreat(0.25f)
    , m_currentPatrolPoint(0)
    , m_currentState(nullptr)
	, m_timerForPatrol(0.f)
	, m_patrolInterval(2.f)
	, m_currentPatrolDirection(0.f, 0.f)
	, m_patrolmaxDistance(300.f)
	, m_spawnPos(0.f, 0.f)
{
    setCategory("Enemy");

    m_currentPatrolDirection = randomDirectionGenerator();
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

    auto animComp = std::make_shared<AnimationComponent>();
    addComponent(animComp);

    m_speed = speed;

    m_spawnPos = position;
}

void IEnemy::update(const float& deltaTime)
{
    if (m_isDead)
    {
        updateAnimation();
        CompositeGameObject::update(deltaTime);
        return;
    }

    if (m_knockbackDuration > 0)
    {
        m_knockbackDuration -= deltaTime;

        auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (renderer)
        {
            sf::Vector2f currentPos = renderer->getPosition();

            float slowdownFactor = std::min(1.0f, 3.0f * deltaTime);
            m_knockBack *= (1.0f - slowdownFactor);

            renderer->setPosition(currentPos + m_knockBack * deltaTime);
        }
    }
    else if (m_currentState)
        m_currentState->update(this, deltaTime);

    updateAnimation();
    updateVisualDirection();

    CompositeGameObject::update(deltaTime);
}

void IEnemy::changeState(IEnemyState* newState)
{
    if (m_currentState)
    {
        m_currentState->exitState(this);
        delete m_currentState;
    }

    m_currentState = newState;
    if (m_currentState)
        m_currentState->enterState(this);
}

void IEnemy::updateAnimation()
{
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (renderer && animComp)
    {
        sf::Vector2f position = renderer->getPosition();
        animComp->updatePosition(position);
    }
}

void IEnemy::updateVisualDirection()
{
    if (!m_isPlayerDetected)
        return;

    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!renderer || !animComp)
        return;

    sf::Vector2f enemyPos = renderer->getPosition();

    float dx = m_playerPos.x - enemyPos.x;
    float dy = m_playerPos.y - enemyPos.y;

    if (std::abs(dx) > std::abs(dy))
        m_currentDirection = (dx > 0) ? EnemyDirection::Right : EnemyDirection::Left;
    else
        m_currentDirection = (dy > 0) ? EnemyDirection::Down : EnemyDirection::Up;

    if (m_currentDirection == EnemyDirection::Left)
        m_isFacingLeft = true;
    else if (m_currentDirection == EnemyDirection::Right)
        m_isFacingLeft = false;

    std::string currentAnim = animComp->getCurrentAnimation();
    std::string statePrefix;

    size_t underscorePos = currentAnim.find('_');
    if (underscorePos != std::string::npos)
        statePrefix = currentAnim.substr(0, underscorePos);
    else
        statePrefix = "run";

    std::string newAnimName;
    switch (m_currentDirection)
    {
    case EnemyDirection::Up:
        newAnimName = statePrefix + "_up";
        break;
    case EnemyDirection::Down:
        newAnimName = statePrefix + "_down";
        break;
    case EnemyDirection::Left:
    case EnemyDirection::Right:
        newAnimName = statePrefix + "_right";
        break;
    }

    if (newAnimName != currentAnim)
        animComp->playAnimation(newAnimName);

    if ((m_currentDirection == EnemyDirection::Left || m_currentDirection == EnemyDirection::Right))
    {
        sf::Vector2f scale = animComp->getScale();
        float absScale = std::abs(scale.x);

        if (m_isFacingLeft)
            animComp->setScale(sf::Vector2f(-absScale, scale.y));
        else
            animComp->setScale(sf::Vector2f(absScale, scale.y));
    }
}

void IEnemy::knockBack(const sf::Vector2f& pos, float force)
{
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!renderer)
        return;

    sf::Vector2f enemyPos = renderer->getPosition();

    sf::Vector2f direction = enemyPos - pos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0)
    {
        direction.x /= length;
        direction.y /= length;
    }
    else
		direction = sf::Vector2f(0, -1);

    m_knockBack = direction * force;
    m_knockbackDuration = 0.3f;
}

bool IEnemy::isDead() const
{
    return m_isDead;
}

bool IEnemy::isNotAttacking() const
{
    auto meleeRetreat = dynamic_cast<const MeleeEnemyStates::RetreatState*>(m_currentState);
    auto rangedRetreat = dynamic_cast<const RangedEnemyStates::RetreatState*>(m_currentState);

    auto positioningState = dynamic_cast<const RangedEnemyStates::PositioningState*>(m_currentState);

    return (meleeRetreat != nullptr || rangedRetreat != nullptr || positioningState != nullptr);
}

void IEnemy::setSpawnPosition(const sf::Vector2f& pos)
{
    m_spawnPos = pos;
}

sf::Vector2f IEnemy::randomDirectionGenerator()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    sf::Vector2f direction(distribution(gen), distribution(gen));

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.f)
        direction /= length;
    else
        direction = sf::Vector2f(1.f, 0.f);

    return direction;
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
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!renderer)
        return;

    sf::Vector2f currentPos = renderer->getPosition();

    m_timerForPatrol += deltaTime;

    if (m_timerForPatrol >= m_patrolInterval)
    {
        m_currentPatrolDirection = randomDirectionGenerator();
        m_timerForPatrol = 0.f;
    }

    sf::Vector2f toSpawn = m_spawnPos - currentPos;
    float distToSpawn = std::sqrt(toSpawn.x * toSpawn.x + toSpawn.y * toSpawn.y);

    if (distToSpawn > m_patrolmaxDistance)
    {
        toSpawn /= distToSpawn;

        m_currentPatrolDirection = toSpawn * 0.7f + m_currentPatrolDirection * 0.3f;

        // Normaliser
        float length = std::sqrt(m_currentPatrolDirection.x * m_currentPatrolDirection.x + m_currentPatrolDirection.y * m_currentPatrolDirection.y);
        if (length > 0.f)
            m_currentPatrolDirection /= length;
    }

    renderer->setPosition(currentPos + m_currentPatrolDirection * m_speed * deltaTime);
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
    if (m_isDead)
        return;

    m_health = std::max(0, m_health - amount);
    std::cout << getName() << " took " << amount << " damage! Health: " << m_health << "/" << m_maxHealth << std::endl;

    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (m_health <= 0)
    {
        m_isDead = true;

        if (animComp)
            animComp->playAnimation("death");

        std::cout << getName() << " has been defeated!" << std::endl;

        auto hitbox = static_cast<Hitbox*>(getComponent("Hitbox"));
        if (hitbox)
            hitbox->setDebugDraw(false);
    }
    else
    {
        if (animComp)
            animComp->playAnimation("hurt");
    }

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