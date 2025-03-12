#include <iostream>

#include "Hero.h"

Hero::Hero()
    : m_health(100)
    , m_armor(10)
    , m_strength(15)
    , m_mana(50)
    , m_stamina(100)
    , m_speed(200.0f)
    , m_velocity(0.0f)
{
    setPosition(sf::Vector2f(400.0f, 300.0f));

    m_stateManager.setState(this, HeroStateNames::stateName::idle);
}

bool Hero::isAlive()
{
    return m_health > 0;
}

bool Hero::isShooting()
{
    return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::attack;
}

bool Hero::isAttacking()
{
    return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::attack;
}

bool Hero::isInvulnerable()
{
    return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::hurt;
}

bool Hero::isIdle() const
{
    return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::idle;
}

void Hero::takeDamage(int damage)
{
    if (isInvulnerable())
        return;

    int actualDamage = std::max(1, damage - m_armor / 2);
    m_health = std::max(0, m_health - actualDamage);

    if (m_health <= 0)
    {
        m_stateManager.setState(this, HeroStateNames::stateName::death);
    }
    else
    {
        m_stateManager.setState(this, HeroStateNames::stateName::hurt);
        setInvulnerable(1.0f);
    }
}

void Hero::setInvulnerable(float duration)
{
    m_stateManager.setState(this, HeroStateNames::stateName::hurt);
}

void Hero::updateInvulnerabilityEffect()
{
    
}

void Hero::attacking()
{
    m_stateManager.setState(this, HeroStateNames::stateName::attack);
}

void Hero::handleInput()
{
    m_stateManager.handleInput(*this);
}

void Hero::update(float deltaTime)
{
    m_stateManager.update(*this, deltaTime);

    if (m_stateManager.getCurrentState()->isTemporaryState())
    {
        
    }

    if (isInvulnerable())
    {
        updateInvulnerabilityEffect();
    }
}

void Hero::setSpeed(float speed)
{
    m_speed = speed;
}

void Hero::move(const sf::Vector2f& offset)
{
    sf::Vector2f currentPos = getPosition();
    setPosition(currentPos + offset);
}

void Hero::setIdle(bool idle)
{
    if (idle && m_stateManager.getCurrentStateName() != HeroStateNames::stateName::idle)
    {
        m_stateManager.setState(this, HeroStateNames::stateName::idle);
    }
}

void Hero::pushState(stateName newState)
{
    m_stateManager.pushState(this, newState);
}

void Hero::popState()
{
    m_stateManager.popState(this);
}

int Hero::getHp()
{
    return m_health;
}

void Hero::getStat()
{
    std::cout << "Hero Stats:" << std::endl;
    std::cout << "HP: " << m_health << std::endl;
    std::cout << "Armor: " << m_armor << std::endl;
    std::cout << "Strength: " << m_strength << std::endl;
    std::cout << "Mana: " << m_mana << std::endl;
    std::cout << "Stamina: " << m_stamina << std::endl;
    std::cout << "Speed: " << m_speed << std::endl;
}

sf::FloatRect Hero::getHitbox() const
{
    return getGlobalBounds();
}

sf::Vector2f Hero::getPlayerPosition()
{
    return getPosition();
}

sf::Vector2f Hero::getPlayerCenter()
{
    sf::FloatRect bounds = getHitbox();
    return sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}