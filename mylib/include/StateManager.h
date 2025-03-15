#pragma once
#include <SFML/Graphics.hpp>

class Hero;

class IState
{
public:
    IState(Hero* owner);
    virtual ~IState() = default;

    virtual void enter() = 0;
    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void exit() = 0;

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
};

// State implementations
class IdleState : public IState
{
public:
    IdleState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
};

class RunState : public IState
{
public:
    RunState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
};

class AttackState : public IState
{
public:
    AttackState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
};

class ShootState : public IState
{
public:
    ShootState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
};

class HurtState : public IState
{
public:
    HurtState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
    const float m_hurtDuration = 0.5f;
};

class DeathState : public IState
{
public:
    DeathState(Hero* owner);
    void enter() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    sf::Clock m_animClock;
};