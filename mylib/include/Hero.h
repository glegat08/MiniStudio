#pragma once
#include "Composite.h"
#include "Component.h"
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

    void loadTextures();
    void initializeAnimations();
    void updateAnimationPosition();

    void update(const float& deltaTime) override;
    void processInput(const sf::Event& event) override;

    bool isAlive() const;
    bool isFacingLeft() const;

    void takeDamage(int amount);
    void move(const sf::Vector2f& offset);
    void setFacingLeft(bool left);
    void setDirection(Direction dir);
    Direction getDirection() const;

    void attack();
    void shoot();

    stateName getCurrentState() const;
    void setState(stateName newState);

    float getSpeed() const;

    HeroState& getStateManager();

private:
    void downAnimation();
    void upAnimation();
    void rightAnimation();
    void hurtAnimation();
    void deathAnimation();

private:
    int m_health;
    int m_armor;
    int m_strength;

    stateName m_currentStateName;
    Direction m_currentDirection;

    bool m_isFacingLeft;

    float m_speed;

    HeroState m_stateManager;

private:
    const int idleFrameCount = 2;
    const int runFrameCount = 2;
    const int shootFrameCount = 4;
    const int attackFrameCount = 6;
    const int hurtFrameCount = 1;
    const int deathFrameCount = 18;

    const int downRow = 0;
    const int rightRow = 1;
    const int upRow = 2;
    const int deathRow = 3;
    const int hurtRow = 4;

    const int idleOffset = 0;
    const int runOffset = idleFrameCount;
    const int shootOffset = idleFrameCount + runFrameCount;
    const int attackOffset = idleFrameCount + runFrameCount + shootFrameCount;
};