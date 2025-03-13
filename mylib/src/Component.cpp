#include "Component.h"
#include "Composite.h"

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
    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            m_isMovingUp = true;
            break;
        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            m_isMovingDown = true;
            break;
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            m_isMovingLeft = true;
            break;
        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            m_isMovingRight = true;
            break;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Z:
        case sf::Keyboard::Up:
            m_isMovingUp = false;
            break;
        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            m_isMovingDown = false;
            break;
        case sf::Keyboard::Q:
        case sf::Keyboard::Left:
            m_isMovingLeft = false;
            break;
        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            m_isMovingRight = false;
            break;
        }
    }
}

void PlayerController::update(const float& deltaTime)
{
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

    auto renderer = static_cast<SquareRenderer*>(m_owner->getComponent("SquareRenderer"));
    if (renderer)
    {
        sf::Vector2f currentPos = renderer->getPosition();
        sf::Vector2f newPos = currentPos + m_velocity * deltaTime;
        renderer->setPosition(newPos);
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