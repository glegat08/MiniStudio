#include "StateManager.h"
#include "Hero.h"
#include "Animation.h"

bool IState::m_mouseLeftPressed = false;

// Base State implementation
IState::IState(Hero* owner)
    : m_owner(owner)
{
}

bool IState::isGoingLeft() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

bool IState::isGoingRight() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

bool IState::isGoingUp() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

bool IState::isGoingDown() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool IState::isAttacking() const
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool result = currentPressed && !m_mouseLeftPressed;
    m_mouseLeftPressed = currentPressed;
    return result;
}

bool IState::isShooting() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void IState::updateDirection()
{
    if (isGoingLeft())
        m_owner->setDirection(Direction::Left);
    else if (isGoingRight())
        m_owner->setDirection(Direction::Right);
    else if (isGoingUp())
        m_owner->setDirection(Direction::Up);
    else if (isGoingDown())
        m_owner->setDirection(Direction::Down);
}

// Idle State Implementation
IdleState::IdleState(Hero* owner)
    : IState(owner)
{
}

void IdleState::enter()
{
    m_owner->setState(stateName::idle);
}

void IdleState::handleInput()
{
    bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

    if (moving) 
    {
        updateDirection();
        m_owner->getStateManager().changeState("Run");
    }

    if (isAttacking()) 
        m_owner->getStateManager().changeState("Attack");

    if (isShooting()) 
        m_owner->getStateManager().changeState("Shoot");
}

void IdleState::update(float deltaTime)
{
}

void IdleState::exit()
{
}

// Run State Implementation
RunState::RunState(Hero* owner)
    : IState(owner)
{
}

void RunState::enter()
{
    m_owner->setState(stateName::run);
}

void RunState::handleInput()
{
    bool opposingHorizontal = isGoingLeft() && isGoingRight();
    bool opposingVertical = isGoingUp() && isGoingDown();
    bool noMovement = !isGoingLeft() && !isGoingRight() && !isGoingUp() && !isGoingDown();

    if (noMovement || opposingHorizontal || opposingVertical)
    {
        m_owner->getStateManager().changeState("Idle");
        return;
    }

    if (isAttacking())
    {
        m_owner->getStateManager().changeState("Attack");
        return;
    }

    if (isShooting())
    {
        m_owner->getStateManager().changeState("Shoot");
        return;
    }
}

void RunState::update(float deltaTime)
{
    if (m_owner->getCurrentState() != stateName::run)
        return;

    if (isGoingUp())
    {
        m_owner->setDirection(Direction::Up);
        m_owner->setState(stateName::run);
    }
    else if (isGoingDown())
    {
        m_owner->setDirection(Direction::Down);
        m_owner->setState(stateName::run);
    }
    else if (isGoingLeft())
    {
        m_owner->setDirection(Direction::Left);
        m_owner->setFacingLeft(true);
        m_owner->setState(stateName::run);
    }
    else if (isGoingRight())
    {
        m_owner->setDirection(Direction::Right);
        m_owner->setFacingLeft(false);
        m_owner->setState(stateName::run);
    }
}

void RunState::exit()
{
}

// Attack State Implementation
AttackState::AttackState(Hero* owner)
    : IState(owner)
{
}

void AttackState::enter()
{
    m_owner->setState(stateName::attack);
    m_animClock.restart();
}

void AttackState::handleInput()
{
}

void AttackState::update(float deltaTime)
{
    auto animComp = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (animComp && animComp->isAnimationFinished()) 
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void AttackState::exit()
{
}

// Shoot State Implementation
ShootState::ShootState(Hero* owner)
    : IState(owner)
{
}

void ShootState::enter()
{
    m_owner->setState(stateName::shoot);
    m_animClock.restart();
}

void ShootState::handleInput()
{
}

void ShootState::update(float deltaTime)
{
    auto animComp = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (animComp && animComp->isAnimationFinished()) 
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void ShootState::exit()
{
}

// Hurt State Implementation
HurtState::HurtState(Hero* owner)
    : IState(owner)
{
}

void HurtState::enter()
{
    m_owner->setState(stateName::hurt);
    m_animClock.restart();
}

void HurtState::handleInput()
{
}

void HurtState::update(float deltaTime)
{
    if (m_animClock.getElapsedTime().asSeconds() >= m_hurtDuration ||
        static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"))->isAnimationFinished()) 
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void HurtState::exit()
{
}

// Death State Implementation
DeathState::DeathState(Hero* owner)
    : IState(owner)
{
}

void DeathState::enter()
{
    m_owner->setState(stateName::death);
}

void DeathState::handleInput()
{
}

void DeathState::update(float deltaTime)
{
}

void DeathState::exit()
{
}