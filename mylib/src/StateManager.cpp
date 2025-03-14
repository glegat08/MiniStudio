#include <SFML/Graphics.hpp>
#include <cmath>

#include "StateManager.h"
#include "hero.h"

// ISTATE
bool IState::isGoingUp()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

bool IState::isGoingDown()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool IState::isGoingLeft()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

bool IState::isGoingRight()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

bool IState::isMeleAttacking()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

bool IState::isRangeAttacking()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Right);
}

void IState::updateDirection(Hero& hero)
{
    if (isGoingRight() && !isGoingLeft())
    {
        hero.setFacingLeft(false);
        hero.getSprite().setScale(2.f, 2.f);
    }
    else if (isGoingLeft() && !isGoingRight())
    {
        hero.setFacingLeft(true);
        hero.getSprite().setScale(-2.f, 2.f);
    }
    else if (isGoingUp() && !isGoingDown())
    {
        hero.setFacingUp(true);
        hero.getSprite().setScale(2.f, 2.f);
    }
    else if (isGoingDown() && !isGoingUp())
    {
        hero.setFacingUp(false);
        hero.getSprite().setScale(2.f, 2.f);
    }
}

bool IState::isTemporaryState() const
{
    return false;
}

// IDLE STATE
void Idle::handleInput(Hero& hero)
{
    if (isGoingUp())
    {
        hero.pushState(HeroStateNames::stateName::move);
        m_currentFrame = 29;
    }
    if (isGoingDown())
    {
        hero.pushState(HeroStateNames::stateName::move);
        m_currentFrame = 1;
    }
    if (isGoingLeft())
    {
        hero.pushState(HeroStateNames::stateName::move);
        m_currentFrame = 15;
    }
    if (isGoingRight())
    {
        hero.pushState(HeroStateNames::stateName::move);
        m_currentFrame = 15;
    }

    updateDirection(hero);

    if (isMeleAttacking())
    {
        hero.pushState(HeroStateNames::stateName::mele_attack);
    }

    if (isRangeAttacking())
    {
        hero.pushState(HeroStateNames::stateName::range_attack);
    }
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
    if (!isGoingRight() && !isGoingLeft() && !isGoingUp() && !isGoingDown())
        hero.setState(HeroStateNames::stateName::idle);

    if (isMeleAttacking())
    {
        hero.pushState(HeroStateNames::stateName::mele_attack);
    }

    if (isRangeAttacking())
    {
        hero.pushState(HeroStateNames::stateName::range_attack);
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
            m_currentFrame = 17;
            updateDirection(hero);
        }
        else if (isGoingLeft() && !isGoingRight())
        {
            hero.move(sf::Vector2f(-hero.getSpeed() * clampedDeltaTime, 0));
            m_currentFrame = 17;
            updateDirection(hero);
        }
        else if (isGoingUp() && !isGoingDown())
        {
            hero.move(sf::Vector2f(0, hero.getSpeed() * clampedDeltaTime));
            m_currentFrame = 31;
            updateDirection(hero);
        }
        else if (isGoingDown() && !isGoingUp())
        {
            hero.move(sf::Vector2f(0, -hero.getSpeed() * clampedDeltaTime));
            m_currentFrame = 3;
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