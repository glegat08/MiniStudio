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
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_frameTime)
    {
        m_currentFrame = (m_currentFrame + 1) % m_frameCount;
        m_elapsedTime.restart();

        sf::IntRect frameRect(m_currentFrame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
        //hero.getSprite().setTextureRect(frameRect);
    }
}

void Idle::setTexture(Hero& hero)
{
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::idle));
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
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::move));
}

// DASH STATE
void Dash::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void Dash::update(Hero& hero, float deltaTime)
{
    if (m_elapsedTime.getElapsedTime().asSeconds() < 0.001f)
    {
        m_dashDirection = calculateMovementDirection();

        if (m_dashDirection.x == 0 && m_dashDirection.y == 0)
        {
            m_dashDirection = sf::Vector2f(1, 0);
        }

        IState::m_dashAvailable = false;
        IState::m_dashCooldownClock.restart();
    }

    float dashSpeed = hero.getSpeed() * DASH_SPEED_MULTIPLIER;
    hero.move(m_dashDirection * dashSpeed * deltaTime);

    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_dashDuration)
    {
        hero.popState();
        m_elapsedTime.restart();
    }
}

void Dash::setTexture(Hero& hero)
{
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::dash));
}

// ATTACK STATE
void Attack::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void Attack::update(Hero& hero, float deltaTime)
{
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_attackDuration) 
    {
        hero.popState();
    }
}

void Attack::setTexture(Hero& hero)
{
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::attack));
}

// HURT STATE
void Hurt::handleInput(Hero& hero)
{
    if (!hero.isInvulnerable())
    {
        if (isGoingUp() || isGoingDown() || isGoingLeft() || isGoingRight())
            hero.pushState(HeroStateNames::stateName::move);
        else
            hero.pushState(HeroStateNames::stateName::idle);
    }
}

void Hurt::update(Hero& hero, float deltaTime)
{
    
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_hurtDuration) 
    {
        hero.popState();
    }
}

void Hurt::setTexture(Hero& hero)
{
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::hurt));
}

// DEATH STATE
void Death::handleInput(Hero& hero)
{
}

void Death::update(Hero& hero, float deltaTime)
{
    if (!m_animationComplete && m_elapsedTime.getElapsedTime().asSeconds() >= m_deathDuration) 
    {
        m_animationComplete = true;
    }
}

void Death::setTexture(Hero& hero)
{
    //hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::death));
}