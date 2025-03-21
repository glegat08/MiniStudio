#pragma once
#include "Composite.h"
#include "HeroState.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

enum class stateName
{
    idle,
    run,
    attack,
    shoot,
    hurt,
    death
};

class Hero : public CompositeGameObject
{
public:
    Hero(const std::string& name = "Hero");
    ~Hero() = default;

    void initialize(const sf::Vector2f& position, const float& size,
    const sf::Color& color, const float& speed);

    void updateAnimationPosition();

    void update(const float& deltaTime) override;
    void processInput(const sf::Event& event) override;

    bool isAlive() const;
    bool isFacingLeft() const;

    void takeDamage(int amount, const sf::Vector2f& attackerPos);
    void move(const sf::Vector2f& offset);
    void setFacingLeft(bool left);
    void setDirection(Direction direction);
    Direction getDirection() const;

    void attack();
    void shoot();

    stateName getCurrentState() const;
    void setState(stateName newState);

    float getSpeed() const;

    HeroState& getStateManager();

    void knockBack(const sf::Vector2f& pos, float force);

    int getHealth() const;
    int getMaxHealth() const;
    float getHealthPercentage() const;
    void setMaxHealth(int maxHealth);
    void heal(int amount);

private:
    int m_health;
    int m_maxHealth;
    int m_armor;
    int m_strength;

    stateName m_currentStateName;
    Direction m_currentDirection;

    bool m_isFacingLeft;

    float m_speed;

    HeroState m_stateManager;

private:
    sf::Vector2f m_knockBack;
    float m_knockBackDuration = 0.0f;
};