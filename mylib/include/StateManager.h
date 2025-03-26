#pragma once
#include <SFML/Graphics.hpp>

class Hero;

class IState
{
public:
    IState(Hero* owner);
    virtual ~IState() = default;

    virtual void enterState() = 0;
    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void exitState() = 0;

    virtual void configureAnimation() = 0;
    virtual void playStateAnimation() = 0;

protected:
    bool isGoingLeft() const;
    bool isGoingRight() const;
    bool isGoingUp() const;
    bool isGoingDown() const;
    bool isAttacking() const;
    bool isShooting() const;

    void updateDirection();

    Hero* m_owner;
    static bool m_mouseLeftPressed;

    const int idleFrameCount = 2;
    const int runFrameCount = 4;
    const int shootFrameCount = 6;
    const int attackFrameCount = 6;
    const int hurtFrameCount = 1;
    const int deathFrameCount = 19;

    const int downRow = 0;
    const int rightRow = 1;
    const int upRow = 2;
    const int deathRow = 3;
    const int hurtRow = 4;

    const int idleOffset = 0;
    const int runOffset = idleFrameCount;
    const int shootOffset = runOffset + runFrameCount;
    const int attackOffset = shootOffset + shootFrameCount;
    const int deathOffset = attackOffset + attackFrameCount;
    const int hurtOffset = deathOffset + deathFrameCount;
};

// State implementations
class IdleState : public IState
{
public:
    IdleState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

private:
    sf::Clock m_animClock;
};

class RunState : public IState
{
public:
    RunState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

private:
    sf::Clock m_animClock;
};

class AttackState : public IState
{
public:
    AttackState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

    void disableMovement();

private:
    sf::Clock m_animClock;
    sf::Vector2f m_initialVelocity;
    float m_decelerationRate;
};

class ShootState : public IState
{
public:
    ShootState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

private:
    sf::Clock m_animClock;
};

class HurtState : public IState
{
public:
    HurtState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

private:
    sf::Clock m_animClock;
    const float m_hurtDuration = 0.5f;
};

class DeathState : public IState
{
public:
    DeathState(Hero* owner);
    void enterState() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exitState() override;

    void configureAnimation() override;
    void playStateAnimation() override;

private:
    sf::Clock m_animClock;
};