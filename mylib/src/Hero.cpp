#include <iostream>

#include "Hero.h"
#include "PathManager.h"

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

void Hero::setState(stateName newState)
{
    m_currentStateName = newState;
    m_stateManager.setState(this, newState);
    m_sprites.setTexture(m_textures[newState]);
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
    m_textures[stateName::idle].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::move].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::dash].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::mele_attack].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::range_attack].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::hurt].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
    m_textures[stateName::death].loadFromFile(PathManager::getResourcePath("hero\\spritesheet_orc.png"));
}

sf::FloatRect Hero::getHitbox() const
{
    sf::FloatRect spriteRect = m_sprites.getGlobalBounds();
    float width, height, offsetX, y;

    // IDLE STATE
    width = spriteRect.width * 0.2f;
    height = spriteRect.height * 1.0f;

    if (m_isIdle)
        offsetX = spriteRect.width * 0.75f;
    else
        offsetX = spriteRect.width * 0.05f;

    y = spriteRect.top + (spriteRect.height - height) * 0.35f;

    switch (m_currentStateName)
    {
    case stateName::move:
        width = spriteRect.width * 0.3f;
        if (m_isMoving)
            offsetX = spriteRect.width * 0.65f;
        else
            offsetX = spriteRect.width * 0.05f;
        break;
    case stateName::mele_attack:
        width = spriteRect.width * 0.47f;
        if (m_isMeleAttacking)
            offsetX = spriteRect.width * 0.47f;
        else
            offsetX = spriteRect.width * 0.05f;
        break;
    case stateName::range_attack:
        width = spriteRect.width * 0.54f;
        if (m_isRangeAttacking)
            offsetX = spriteRect.width * 0.4f;
        else
            offsetX = spriteRect.width * 0.05f;
        break;
    case stateName::dash:
        width = spriteRect.width * 0.5f;
        if (m_isDashing)
            offsetX = spriteRect.width * 0.45f;
        else
            offsetX = spriteRect.width * 0.05f;
        break;
    
    }

    float x = spriteRect.left + offsetX;

    return sf::FloatRect(x, y, width, height);
}

sf::Vector2f Hero::getPlayerPosition()
{
    return m_sprites.getPosition();
}

sf::Vector2f Hero::getPlayerCenter()
{
    return { getHitbox().left + getHitbox().width / 2.f, getHitbox().top + getHitbox().height / 2.f };
}