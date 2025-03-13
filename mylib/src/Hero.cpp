#include <iostream>

#include "Hero.h"
#include "TextureManager.h"

Hero::Hero()
{
    setStateTexture();
	m_sprites.setScale(2.f, 2.f);
}

bool Hero::isAlive()
{
    return m_health > 0;
}

bool Hero::isMeleAttacking()
{
    return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::mele_attack;
}

bool Hero::isRangeAttacking()
{
	return m_stateManager.getCurrentStateName() == HeroStateNames::stateName::range_attack;
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
    m_isInvulnerable = true;
    m_invulnerabilityTimer.restart();
    m_invulnerabilityDuration = duration;
}

void Hero::updateInvulnerabilityEffect()
{
    if (m_isInvulnerable && m_invulnerabilityTimer.getElapsedTime().asSeconds() >= m_invulnerabilityDuration)
        m_isInvulnerable = false;   
}

void Hero::meleAttacking()
{
    m_stateManager.setState(this, HeroStateNames::stateName::mele_attack);
}

void Hero::rangeAttacking()
{
    m_stateManager.setState(this, HeroStateNames::stateName::range_attack);
}

void Hero::handleInput()
{
    m_stateManager.handleInput(*this);
}

void Hero::update(float deltaTime)
{
    updateInvulnerabilityEffect();
    m_stateManager.update(*this, deltaTime);
}

void Hero::setSpeed(float speed)
{
    m_speed = speed;
}

int Hero::getHp()
{
    return m_health;
}

int Hero::getStrenght() const
{
    return m_strength;
}

float Hero::getSpeed() const
{
    return m_speed;
}

void Hero::move(const sf::Vector2f& offset)
{
    m_sprites.move(offset);
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

sf::Texture& Hero::getTexture(const stateName& stateName_)
{
    if (m_textures.find(stateName_) == m_textures.end())
        return m_textures[stateName::idle];

    return m_textures[stateName_];
}

sf::Sprite& Hero::getSprite()
{
    return m_sprites;
}

Hero::stateName Hero::getCurrentState() const
{
    return m_currentStateName;
}

HeroState& Hero::getStateManager()
{
    return m_stateManager;
}

void Hero::setStateTexture()
{
    /*m_textures[stateName::idle].loadFromFile(PathManager::getResourcePath("hero\\IDLE.png"));
    m_textures[stateName::move].loadFromFile(PathManager::getResourcePath("hero\\MOVE.png"));
    m_textures[stateName::dash].loadFromFile(PathManager::getResourcePath("hero\\DASH.png"));
    m_textures[stateName::mele_attack].loadFromFile(PathManager::getResourcePath("hero\\MELEATTACK.png"));
    m_textures[stateName::range_attack].loadFromFile(PathManager::getResourcePath("hero\\RANGEATTACK.png"));
    m_textures[stateName::hurt].loadFromFile(PathManager::getResourcePath("hero\\HURT.png"));
    m_textures[stateName::death].loadFromFile(PathManager::getResourcePath("hero\\DEATH.png"));*/
}

sf::FloatRect Hero::getHitbox() const
{
    //return getGlobalBounds();
}

sf::Vector2f Hero::getPlayerPosition()
{
    //return getPosition();
}

sf::Vector2f Hero::getPlayerCenter()
{
    sf::FloatRect bounds = getHitbox();
    return sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
}