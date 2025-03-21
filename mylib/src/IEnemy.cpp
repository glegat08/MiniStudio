#include "IEnemy.h"
#include <cmath>
#include <iostream>
#include <random>

#include "Animation.h"
#include "Collision.h"
#include "Effect.h"
#include "Enemy.h"

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
    auto square_renderer = std::make_shared<SquareRenderer>(size, color);
    addComponent(square_renderer);
    square_renderer->setPosition(position);

    auto animation_component = std::make_shared<AnimationComponent>();
    addComponent(animation_component);

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

        auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (square_renderer)
        {
            sf::Vector2f currentPos = square_renderer->getPosition();

            float slowdown_factor = std::min(1.0f, 3.0f * deltaTime);
            m_knockBack *= (1.0f - slowdown_factor);

            square_renderer->setPosition(currentPos + m_knockBack * deltaTime);
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
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (square_renderer && animation_component)
    {
        sf::Vector2f position = square_renderer->getPosition();
        animation_component->updatePosition(position);
    }
}

void IEnemy::updateVisualDirection()
{
    if (!m_isPlayerDetected)
        return;

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!square_renderer || !animation_component)
        return;

    sf::Vector2f enemyPos = square_renderer->getPosition();

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

    std::string current_anim = animation_component->getCurrentAnimation();
    std::string statePrefix;

    size_t underscore_pos = current_anim.find('_');
    if (underscore_pos != std::string::npos)
        statePrefix = current_anim.substr(0, underscore_pos);
    else
        statePrefix = "run";

    std::string new_anim_name;
    switch (m_currentDirection)
    {
    case EnemyDirection::Up:
        new_anim_name = statePrefix + "_up";
        break;
    case EnemyDirection::Down:
        new_anim_name = statePrefix + "_down";
        break;
    case EnemyDirection::Left:
    case EnemyDirection::Right:
        new_anim_name = statePrefix + "_right";
        break;
    }

    if (new_anim_name != current_anim)
        animation_component->playAnimation(new_anim_name);

    if ((m_currentDirection == EnemyDirection::Left || m_currentDirection == EnemyDirection::Right))
    {
        sf::Vector2f scale = animation_component->getScale();
        float absScale = std::abs(scale.x);

        if (m_isFacingLeft)
            animation_component->setScale(sf::Vector2f(-absScale, scale.y));
        else
            animation_component->setScale(sf::Vector2f(absScale, scale.y));
    }
}

void IEnemy::knockBack(const sf::Vector2f& pos, float force)
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f enemyPos = square_renderer->getPosition();

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
    auto melee_retreat = dynamic_cast<const MeleeEnemyStates::RetreatState*>(m_currentState);
    auto ranged_retreat = dynamic_cast<const RangedEnemyStates::RetreatState*>(m_currentState);

    auto positioningState = dynamic_cast<const RangedEnemyStates::PositioningState*>(m_currentState);

    return (melee_retreat != nullptr || ranged_retreat != nullptr || positioningState != nullptr);
}

void IEnemy::setSpawnPosition(const sf::Vector2f& pos)
{
    m_spawnPos = pos;
}

void IEnemy::heroIsDead(std::vector<std::shared_ptr<CompositeGameObject>>& gameObjects)
{
    for (auto& object : gameObjects)
    {
        if (object->getCategory() == "Enemy")
        {
            auto enemy = dynamic_cast<IEnemy*>(object.get());
            if (enemy && !enemy->isDead())
            {
                enemy->forgetPlayer();

                auto melee_enemy = dynamic_cast<MeleeEnemy*>(enemy);
                if (melee_enemy)
                    melee_enemy->changeState(new MeleeEnemyStates::PatrolState());

                auto ranged_enemy = dynamic_cast<RangedEnemy*>(enemy);
                if (ranged_enemy)
                    ranged_enemy->changeState(new RangedEnemyStates::PatrolState());
            }
        }
    }
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
    auto square_renderer = static_cast<const SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer || !m_isPlayerDetected)
        return false;

    sf::Vector2f pos = square_renderer->getPosition();
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
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();

    m_timerForPatrol += deltaTime;

    if (m_timerForPatrol >= m_patrolInterval)
    {
        m_currentPatrolDirection = randomDirectionGenerator();
        m_timerForPatrol = 0.f;
    }

    sf::Vector2f to_spawn = m_spawnPos - currentPos;
    float dist_to_spawn = std::sqrt(to_spawn.x * to_spawn.x + to_spawn.y * to_spawn.y);

    if (dist_to_spawn > m_patrolmaxDistance)
    {
        to_spawn /= dist_to_spawn;

        m_currentPatrolDirection = to_spawn * 0.7f + m_currentPatrolDirection * 0.3f;

        float length = std::sqrt(m_currentPatrolDirection.x * m_currentPatrolDirection.x + m_currentPatrolDirection.y * m_currentPatrolDirection.y);
        if (length > 0.f)
            m_currentPatrolDirection /= length;
    }

    square_renderer->setPosition(currentPos + m_currentPatrolDirection * m_speed * deltaTime);
}

void IEnemy::moveTowardPlayer(float deltaTime)
{
    if (!m_isPlayerDetected)
        return;

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();
    sf::Vector2f direction = m_playerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= m_attackRadius * 0.8f)
        return;

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * deltaTime;
        square_renderer->setPosition(currentPos + direction);
    }
}

void IEnemy::retreatFromPlayer(float deltaTime)
{
    if (!m_isPlayerDetected)
        return;

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();
    sf::Vector2f direction = currentPos - m_playerPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0)
    {
        direction /= distance;
        direction *= m_speed * 1.2f * deltaTime;
        square_renderer->setPosition(currentPos + direction);
    }
}

void IEnemy::searchingArea(float deltaTime)
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();
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
        square_renderer->setPosition(currentPos + direction);
    }
}

void IEnemy::takeDamage(int amount, const sf::Vector2f& attackerPos)
{
    if (m_isDead)
        return;

    m_health = std::max(0, m_health - amount);
    std::cout << getName() << " took " << amount << " damage! Health: " << m_health << "/" << m_maxHealth << std::endl;

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (m_health <= 0)
    {
        m_isDead = true;

        if (animation_component)
            animation_component->playAnimation("death");

        std::cout << getName() << " has been defeated!" << std::endl;

        auto hitbox = static_cast<Hitbox*>(getComponent("Hitbox"));
        if (hitbox)
            hitbox->setDebugDraw(false);
    }
    else
    {
        if (animation_component)
            animation_component->playAnimation("hurt");
    }

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (square_renderer)
    {
        sf::Vector2f position = square_renderer->getPosition();

        sf::Vector2f bloodDirection = position - attackerPos;
    	float length = std::sqrt(bloodDirection.x * bloodDirection.x + bloodDirection.y * bloodDirection.y);
        if (length > 0)
        {
            bloodDirection.x /= length;
            bloodDirection.y /= length;
        }
        else
            bloodDirection = sf::Vector2f(0, 1);

        BloodEffect::createBloodEffect(position, bloodDirection);
        updateLOS(m_playerPos);
    }
}

void IEnemy::forgetPlayer()
{
    m_isPlayerDetected = false;
}

void IEnemy::updateLOS(const sf::Vector2f& playerPos)
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();
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
    for (auto& object : gameObjects)
    {
        if (object->getCategory() == "Enemy")
        {
            auto enemy = dynamic_cast<IEnemy*>(object.get());
            if (enemy)
                enemy->updateLOS(playerPos);
        }
    }
}