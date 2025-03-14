#pragma once
#include "Composite.h"
#include "EnemyState.h"

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

protected:
    int m_health;
    int m_maxHealth;
    int m_damage;
    float m_speed;

    float m_detectionRadius;
    float m_attackRadius;
    bool m_isPlayerDetected;
    sf::Vector2f m_playerPos;
    sf::Vector2f m_lastKnownPlayerPos;

    float m_healthIsCritical;
    float m_retreat;

    std::vector<sf::Vector2f> m_patrolPoints;
    int m_currentPatrolPoint;

    IEnemyState* m_currentState;
};