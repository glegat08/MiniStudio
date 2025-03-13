#include <SFML/Graphics.hpp>
#include <cmath>

#include "StateManager.h"
#include "hero.h"

bool IState::m_dashAvailable = true;
sf::Clock IState::m_dashCooldownClock;
const float IState::m_dashCooldownDuration = 2.0f;
bool IState::m_mouseRightPressed = false;
bool IState::m_mouseLeftPressed = false;

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

    if (isMeleAttacking())
    {
        hero.pushState(HeroStateNames::stateName::mele_attack);
        return;
    }

    if (isRangeAttacking())
    {
        hero.pushState(HeroStateNames::stateName::range_attack);
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
        hero.getSprite().setTextureRect(frameRect);
    }
}

void Idle::setTexture(Hero& hero)
{
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::idle));
}

// MOVING STATE
void Movement::handleInput(Hero& hero)
{
    if ((!isGoingRight() && !isGoingLeft()) || (isGoingRight() && isGoingLeft()) || (isGoingUp() && isGoingDown() || (!isGoingUp() && !isGoingDown())))
        hero.setState(HeroStateNames::stateName::idle);

    if (isMeleAttacking())
    {
        hero.pushState(HeroStateNames::stateName::mele_attack);
        return;
    }

    if (isRangeAttacking())
    {
        hero.pushState(HeroStateNames::stateName::range_attack);
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
    if (hero.getCurrentState() == HeroStateNames::stateName::move)
    {
        float clampedDeltaTime = std::min(deltaTime, 0.1f);

        if (isGoingRight() && !isGoingLeft())
        {
            hero.move(sf::Vector2f(hero.getSpeed() * clampedDeltaTime, 0));
            updateDirection(hero);
        }
        else if (isGoingLeft() && !isGoingRight())
        {
            hero.move(sf::Vector2f(-hero.getSpeed() * clampedDeltaTime, 0));
            updateDirection(hero);
        }

        if (m_elapsedTime.getElapsedTime().asSeconds() >= m_frameTime)
        {
            m_currentFrame = (m_currentFrame + 1) % m_frameCount;
            m_elapsedTime.restart();
            sf::IntRect currentFrameRect(m_currentFrame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
            hero.getSprite().setTextureRect(currentFrameRect);
        }
    }
}

void Movement::setTexture(Hero& hero)
{
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::move));
}

// DASH STATE
void Dash::handleInput(Hero& hero)
{
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_dashDuration)
    {
        if (!hero.getStateManager().isStateStackEmpty())
            hero.getStateManager().popState(&hero);
        else
        {
            if (isGoingLeft() || isGoingRight())
                hero.setState(HeroStateNames::stateName::move);
            else
                hero.setState(HeroStateNames::stateName::idle);
        }
    }
}

void Dash::update(Hero& hero, float deltaTime)
{
    float clampedDeltaTime = std::min(deltaTime, 0.1f);
    float dashDirection = hero.isIdle() ? -1.0f : 1.0f;

    IState::m_dashCooldownClock.restart();
    IState::m_dashAvailable = false;

    hero.move(sf::Vector2f(m_dashSpeed * dashDirection * clampedDeltaTime, 0));

    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_frameTime)
    {
        m_currentFrame = (m_currentFrame + 1) % m_frameCount;
        m_elapsedTime.restart();
        sf::IntRect currentFrameRect(m_currentFrame * m_frameWidth, 0, m_frameWidth, m_frameHeight);
        hero.getSprite().setTextureRect(currentFrameRect);
    }
}

void Dash::setTexture(Hero& hero)
{
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::dash));
}

// ATTACK STATE
void MeleAttack::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void MeleAttack::update(Hero& hero, float deltaTime)
{
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_meleAttackDuration)
    {
        hero.popState();
    }
}

void MeleAttack::setTexture(Hero& hero)
{
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::mele_attack));
}

// ATTACK STATE
void RangeAttack::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void RangeAttack::update(Hero& hero, float deltaTime)
{
    if (m_elapsedTime.getElapsedTime().asSeconds() >= m_rangeAttackDuration)
    {
        hero.popState();
    }
}

void RangeAttack::setTexture(Hero& hero)
{
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::range_attack));
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
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::hurt));
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
    hero.getSprite().setTexture(hero.getTexture(HeroStateNames::stateName::death));
}