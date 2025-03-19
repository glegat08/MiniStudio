#pragma once
#include "Composite.h"
#include "EnemyState.h"

enum class EnemyDirection
{
    Up,
    Down,
    Left,
    Right
};

class IEnemy : public CompositeGameObject
{
public:
    IEnemy(const std::string& name = "Enemy");
    virtual ~IEnemy();

    void init(const sf::Vector2f& position, const float& size,
        const sf::Color& color, const float& speed);

    void update(const float& deltaTime) override;

    bool isHealthCritical() const;
    bool canDetectPlayer() const;
    bool isWithinAttackRange() const;
    bool shouldRetreat() const;

    void patrol(float deltaTime);
    void moveTowardPlayer(float deltaTime);
    void retreatFromPlayer(float deltaTime);
    void searchingArea(float deltaTime);

    virtual void Attack(float deltaTime) = 0;
    virtual void Block(float deltaTime) = 0;

    void takeDamage(int amount);
    void forgetPlayer();
    void updateLOS(const sf::Vector2f& playerPos);
    static void updateAllEnemyLOS(std::vector<std::shared_ptr<CompositeGameObject>>& gameObjects,
        const sf::Vector2f& playerPos);

    void changeState(IEnemyState* newState);
    void updateAnimation();
    void updateVisualDirection();

    void knockBack(const sf::Vector2f& pos, float force);
    bool isDead() const;
    bool isNotAttacking() const;

    void setSpawnPosition(const sf::Vector2f& pos);

protected:
    int m_health;
    int m_maxHealth;
    int m_damage;
    float m_speed;
    bool m_isDead = false;

    float m_detectionRadius;
    float m_attackRadius;
    bool m_isPlayerDetected;
    EnemyDirection m_currentDirection;
    bool m_isFacingLeft;
    sf::Vector2f m_playerPos;
    sf::Vector2f m_lastKnownPlayerPos;
    sf::Vector2f m_knockBack;
    float m_knockbackDuration = 0.0f;

    float m_healthIsCritical;
    float m_retreat;

    std::vector<sf::Vector2f> m_patrolPoints;
    int m_currentPatrolPoint;

    IEnemyState* m_currentState;

    float m_timerForPatrol;
    float m_patrolInterval;
    float m_patrolmaxDistance;
    sf::Vector2f m_currentPatrolDirection;
    sf::Vector2f m_spawnPos;

    sf::Vector2f randomDirectionGenerator();

protected:
    const int idleFrameCount = 2;
    const int runFrameCount = 4;
    const int attackFrameCount = 4;
    const int hurtFrameCount = 1;
    const int deathFrameCount = 4;

    const int idleDownOffset = 0;
    const int runDownOffset = 2;
    const int attackDownOffset = 6;
    const int hurtDownOffset = 10;

    const int idleRightOffset = 11;
    const int runRightOffset = 13;
    const int attackRightOffset = 17;
    const int hurtRightOffset = 21;

    const int idleUpOffset = 22;
    const int runUpOffset = 24;
    const int attackUpOffset = 28;
    const int hurtUpOffset = 32;

    const int deathOffset = 33;
};
