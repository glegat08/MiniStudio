#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "StateManager.h"
#include "hero.h"

bool IState::m_dashAvailable = true;
sf::Clock IState::m_dashCooldownClock;
const float IState::m_dashCooldownDuration = 2.0f;
bool IState::m_mouseRightPressed = false;
bool IState::m_mouseLeftPressed = false;

const sf::Vector2f DIR_UP(0, -1);
const sf::Vector2f DIR_DOWN(0, 1);
const sf::Vector2f DIR_LEFT(-1, 0);
const sf::Vector2f DIR_RIGHT(1, 0);
const float DASH_SPEED_MULTIPLIER = 3.0f;
const float DASH_DURATION = 0.35f;
const float ATTACK_DURATION = 0.5f;
const float HURT_DURATION = 0.7f;

// ISTATE
bool IState::isGoingUp()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

bool IState::isGoingDown()
{
    return sf::Keyboard::isKeyPressed((sf::Keyboard::S));
}

bool IState::isGoingLeft()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

bool IState::isGoingRight()
{
    return sf::Keyboard::isKeyPressed((sf::Keyboard::D));
}

bool IState::isDashing()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canDash();
}

bool IState::canDash()
{
    if (!m_dashAvailable && m_dashCooldownClock.getElapsedTime().asSeconds() >= m_dashCooldownDuration)
        m_dashAvailable = true;

    return m_dashAvailable;
}

bool IState::isMeleAttacking()
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool result = currentPressed && !m_mouseLeftPressed;
    m_mouseLeftPressed = currentPressed;
    return result;
}

bool IState::isRangeAttacking()
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    bool result = currentPressed && !m_mouseRightPressed;
    m_mouseRightPressed = currentPressed;
    return result;
}

sf::Vector2f calculateMovementDirection()
{
    sf::Vector2f direction(0, 0);

    if (IState::isGoingUp()) direction += DIR_UP;
    if (IState::isGoingDown()) direction += DIR_DOWN;
    if (IState::isGoingLeft()) direction += DIR_LEFT;
    if (IState::isGoingRight()) direction += DIR_RIGHT;

    // Normalize the vector if it's not zero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) 
    {
        direction /= length;
    }

    return direction;
}

void IState::updateDirection(Hero& hero)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(); 
    sf::Vector2f heroPos = hero.getPlayerCenter();

    sf::Vector2f direction = sf::Vector2f(mousePos.x, mousePos.y) - heroPos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) 
    {
        direction /= length;
    }
}

bool IState::isTemporaryState() const
{
    return false;
}

// IDLE STATE
void Idle::handleInput(Hero& hero)
{
    if (isGoingUp() || isGoingDown() || isGoingLeft() || isGoingRight()) 
    {
        hero.pushState(HeroStateNames::stateName::move);
        return;
    }

    if (isMeleAttacking() || isRangeAttacking()) 
    {
        hero.pushState(HeroStateNames::stateName::attack);
        return;
    }

	if (isDashing()) 
    {
        hero.pushState(HeroStateNames::stateName::dash);
        return;
    }

    updateDirection(hero);
}

void Idle::update(Hero& hero, float deltaTime)
{

}

void Idle::setTexture(Hero& hero)
{

}

// MOVING STATE
void Movement::handleInput(Hero& hero)
{
    bool isMoving = isGoingUp() || isGoingDown() || isGoingLeft() || isGoingRight();

    if (!isMoving) 
    {
        hero.popState();
        return;
    }

    if (isMeleAttacking() || isRangeAttacking()) 
    {
        hero.pushState(HeroStateNames::stateName::attack);
        return;
    }

    if (isDashing()) 
    {
        hero.pushState(HeroStateNames::stateName::dash);
        return;
    }

    updateDirection(hero);
}

void Movement::update(Hero& hero, float deltaTime)
{
    sf::Vector2f direction = calculateMovementDirection();

    if (direction.x != 0 || direction.y != 0) 
    {
        float speed = hero.getSpeed();
        hero.move(direction * speed * deltaTime);
    }
}

void Movement::setTexture(Hero& hero)
{
    
}

// DASH STATE
static sf::Clock dashTimer;
static sf::Vector2f dashDirection;

void Dash::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void Dash::update(Hero& hero, float deltaTime)
{
    if (dashTimer.getElapsedTime().asSeconds() < 0.001f) 
    {
        dashDirection = calculateMovementDirection();

        if (dashDirection.x == 0 && dashDirection.y == 0) 
        {
            dashDirection = sf::Vector2f(1, 0);
        }

        IState::m_dashAvailable = false;
        IState::m_dashCooldownClock.restart();
    }

    float dashSpeed = hero.getSpeed() * DASH_SPEED_MULTIPLIER;
    hero.move(dashDirection * dashSpeed * deltaTime);

    if (dashTimer.getElapsedTime().asSeconds() >= DASH_DURATION) 
    {
        hero.popState();
        dashTimer.restart();
    }
}

void Dash::setTexture(Hero& hero)
{

}

// ATTACK STATE
static sf::Clock attackTimer;

void Attack::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void Attack::update(Hero& hero, float deltaTime)
{
    if (attackTimer.getElapsedTime().asSeconds() >= ATTACK_DURATION) 
    {
        hero.popState();
    }
}

void Attack::setTexture(Hero& hero)
{

}

// HURT STATE
static sf::Clock hurtTimer;

void Hurt::handleInput(Hero& hero)
{

}

void Hurt::update(Hero& hero, float deltaTime)
{
    
    if (hurtTimer.getElapsedTime().asSeconds() >= HURT_DURATION) 
    {
        hero.popState();
    }
}

void Hurt::setTexture(Hero& hero)
{

}

// DEATH STATE
static bool deathAnimationComplete = false;
static sf::Clock deathTimer;
const float DEATH_ANIMATION_DURATION = 2.0f;

void Death::handleInput(Hero& hero)
{
    
}

void Death::update(Hero& hero, float deltaTime)
{
    if (!deathAnimationComplete && deathTimer.getElapsedTime().asSeconds() >= DEATH_ANIMATION_DURATION) {
        deathAnimationComplete = true;
    }
}

void Death::setTexture(Hero& hero)
{
    
}