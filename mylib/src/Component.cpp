#include "Component.h"

#include <iostream>

#include "AudioManager.h"
#include "Composite.h"
#include "Hero.h"
#include "Map.h"
#include "PathManager.h"
#include "TextureManager.h"

Component::Component(const std::string& name)
	: m_name(name)
{
}

void Component::initialize()
{
}

void Component::processInput(const sf::Event& event)
{
}

void Component::update(const float& deltaTime)
{
}

void Component::render(sf::RenderWindow& window)
{
}

void Component::setOwner(CompositeGameObject* owner)
{
	m_owner = owner;
}

CompositeGameObject* Component::getOwner() const
{
	return m_owner;
}

std::string Component::getName() const
{
	return m_name;
}

void Component::setName(const std::string& name)
{
	m_name = name;
}


// PLAYER RENDER
SquareRenderer::SquareRenderer(float size, const sf::Color& color)
    : Component("SquareRenderer")
    , m_size(size)
    , m_color(color)
{
}

void SquareRenderer::initialize()
{
    m_shape.setSize(sf::Vector2f(m_size, m_size));
    m_shape.setFillColor(m_color);
    m_shape.setOrigin(m_size / 2.0f, m_size / 2.0f);
}

void SquareRenderer::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

void SquareRenderer::setSize(float size)
{
    m_size = size;
    m_shape.setSize(sf::Vector2f(m_size, m_size));
    m_shape.setOrigin(m_size / 2.0f, m_size / 2.0f);
}

void SquareRenderer::setColor(const sf::Color& color)
{
    m_color = color;
    m_shape.setFillColor(m_color);
}

void SquareRenderer::setPosition(const sf::Vector2f& position)
{
    m_shape.setPosition(position);
}

sf::Vector2f SquareRenderer::getPosition() const
{
    return m_shape.getPosition();
}

// PLAYER MOVE
PlayerController::PlayerController(float speed)
    : Component("PlayerController")
    , m_speed(speed)
    , m_direction(0.0f, 0.0f)
    , m_isMovingUp(false)
    , m_isMovingDown(false)
    , m_isMovingLeft(false)
    , m_isMovingRight(false)
{
}

void PlayerController::initialize()
{
}

void PlayerController::processInput(const sf::Event& event)
{
    m_isMovingUp = false;
    m_isMovingDown = false;
    m_isMovingLeft = false;
    m_isMovingRight = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        m_isMovingUp = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_isMovingDown = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        m_isMovingLeft = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_isMovingRight = true;
}

void PlayerController::update(const float& deltaTime)
{
    auto hero = dynamic_cast<Hero*>(m_owner);
    if (hero && hero->getCurrentState() == stateName::death)
    {
        m_velocity = sf::Vector2f(0.f, 0.f);
        return;
    }


    sf::Vector2f currentVelocity = m_velocity;
    sf::Vector2f targetVelocity(0.0f, 0.0f);

    if (m_isMovingUp)
        targetVelocity.y -= m_speed;
    if (m_isMovingDown)
        targetVelocity.y += m_speed;
    if (m_isMovingLeft)
        targetVelocity.x -= m_speed;
    if (m_isMovingRight)
        targetVelocity.x += m_speed;

    float length = sqrt(targetVelocity.x * targetVelocity.x + targetVelocity.y * targetVelocity.y);
    if (length > 0.0f)
    {
        targetVelocity.x /= length;
        targetVelocity.y /= length;
        targetVelocity *= m_speed;
    }

    float smoothFactor = 10.0f;

    m_velocity.x = currentVelocity.x + (targetVelocity.x - currentVelocity.x) * std::min(1.0f, deltaTime * smoothFactor);
    m_velocity.y = currentVelocity.y + (targetVelocity.y - currentVelocity.y) * std::min(1.0f, deltaTime * smoothFactor);

    auto square_renderer = static_cast<SquareRenderer*>(m_owner->getComponent("SquareRenderer"));
    if (square_renderer)
    {
        sf::Vector2f currentPos = square_renderer->getPosition();
        sf::Vector2f newPos = currentPos + m_velocity * deltaTime;

        float player_radius = 25.f;
		newPos = WorldLimits::limits(newPos, player_radius);
        square_renderer->setPosition(newPos);
    }
}

void PlayerController::setSpeed(float speed)
{
    m_speed = speed;
}

float PlayerController::getSpeed() const
{
    return m_speed;
}

bool PlayerController::isMoving() const
{
    return m_isMovingUp || m_isMovingDown || m_isMovingLeft || m_isMovingRight;
}

bool PlayerController::isMovingUp() const
{
    return m_isMovingUp;
}

bool PlayerController::isMovingDown() const
{
    return m_isMovingDown;
}

bool PlayerController::isMovingLeft() const
{
    return m_isMovingLeft;
}

bool PlayerController::isMovingRight() const
{
    return m_isMovingRight;
}



SoundComponent::SoundComponent(const std::string& name)
    : Component(name)
{
}

void SoundComponent::initialize()
{
    Component::initialize();
}

void SoundComponent::playSound(const std::string& soundName, float volume, float pitch)
{
    SoundManager::getInstance().playSound(soundName, volume, pitch);
}

void SoundComponent::stopSound(const std::string& soundName)
{
    SoundManager::getInstance().stopSound(soundName);
}

void SoundComponent::stopAllSounds()
{
    for (const auto& mapping : m_soundMappings)
    {
        stopSound(mapping.second);
    }
}

void SoundComponent::addSoundMapping(const std::string& actionName, const std::string& soundName)
{
    m_soundMappings[actionName] = soundName;
}

void SoundComponent::removeSoundMapping(const std::string& actionName)
{
    auto it = m_soundMappings.find(actionName);
    if (it != m_soundMappings.end())
    {
        m_soundMappings.erase(it);
    }
}

bool SoundComponent::hasMapping(const std::string& actionName) const
{
    return m_soundMappings.find(actionName) != m_soundMappings.end();
}

void SoundComponent::playAction(const std::string& actionName, float volume, float pitch)
{
    if (hasMapping(actionName))
    {
        playSound(m_soundMappings[actionName], volume, pitch);
    }
    else
    {
        std::cerr << "No sound mapping found for action: " << actionName << std::endl;
    }
}

void SoundComponent::stopAction(const std::string& actionName)
{
    if (hasMapping(actionName))
    {
        stopSound(m_soundMappings[actionName]);
    }
}