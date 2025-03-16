#include "Component.h"
#include "Composite.h"
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

TileComponent::TileComponent(const std::string& tilesetName)
    : Component("TileComponent")
    , m_tilesetName(tilesetName)
    , m_tileId(0)
    , m_gridPosition(0, 0)
    , m_size(32.0f)
    , m_walkable(true)
{
}

void TileComponent::initialize()
{
    Component::initialize();
    updateTextureRect();
}

void TileComponent::update(const float& deltaTime)
{}

void TileComponent::render(sf::RenderWindow& window)
{
    if (m_tileId > 0)
        window.draw(m_sprite);
}

void TileComponent::setTileId(int id)
{
    m_tileId = id;
    updateTextureRect();
}

int TileComponent::getTileId() const
{
    return m_tileId;
}

void TileComponent::setPosition(int gridX, int gridY)
{
    m_gridPosition.x = gridX;
    m_gridPosition.y = gridY;
    m_sprite.setPosition(gridX * m_size, gridY * m_size);
}

sf::Vector2i TileComponent::getGridPosition() const
{
    return m_gridPosition;
}

sf::Vector2f TileComponent::getWorldPosition() const
{
    return sf::Vector2f(m_gridPosition.x * m_size, m_gridPosition.y * m_size);
}

void TileComponent::setSize(float size)
{
    m_size = size;
    m_sprite.setScale(m_size / 32.0f, m_size / 32.0f);  // Assuming default tile size in tileset is 32x32
    m_sprite.setPosition(m_gridPosition.x * m_size, m_gridPosition.y * m_size);
}

float TileComponent::getSize() const
{
    return m_size;
}

bool TileComponent::isWalkable() const
{
    return m_walkable;
}

void TileComponent::setWalkable(bool walkable)
{
    m_walkable = walkable;
}

void TileComponent::setTilesetName(const std::string& tilesetName)
{
    m_tilesetName = tilesetName;
    updateTextureRect();
}

std::string TileComponent::getTilesetName() const
{
    return m_tilesetName;
}

void TileComponent::updateTextureRect()
{
    if (m_tileId <= 0 || m_tilesetName.empty())
        return;

    sf::Texture* texture = TextureManager::getInstance().getTexture(m_tilesetName);
    if (!texture)
        return;

    m_sprite.setTexture(*texture);

    const int tileSize = 32;
    const int tilesPerRow = texture->getSize().x / tileSize;

    int tileX = (m_tileId - 1) % tilesPerRow;
    int tileY = (m_tileId - 1) / tilesPerRow;

    m_textureRect = sf::IntRect(tileX * tileSize, tileY * tileSize, tileSize, tileSize);
    m_sprite.setTextureRect(m_textureRect);

    m_sprite.setOrigin(0.0f, 0.0f);
    m_sprite.setPosition(m_gridPosition.x * m_size, m_gridPosition.y * m_size);
    m_sprite.setScale(m_size / tileSize, m_size / tileSize);
}

TileMap::TileMap(int width, int height, float tileSize)
    : Component("TileMap")
    , m_width(width)
    , m_height(height)
    , m_tileSize(tileSize)
{
    resize(width, height);
}

void TileMap::initialize()
{
    Component::initialize();
}

void TileMap::update(const float& deltaTime)
{}

void TileMap::render(sf::RenderWindow& window)
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            if (m_tiles[y][x])
                m_tiles[y][x]->render(window);
        }
    }
}

void TileMap::setTile(int x, int y, int tileId, const std::string& tilesetName)
{
    if (!isValidPosition(x, y))
        return;

    if (!m_tiles[y][x])
    {
        m_tiles[y][x] = std::make_shared<TileComponent>(tilesetName);
        m_tiles[y][x]->setSize(m_tileSize);
        m_tiles[y][x]->setPosition(x, y);
    }

    m_tiles[y][x]->setTileId(tileId);
    m_tiles[y][x]->setTilesetName(tilesetName);
}

TileComponent* TileMap::getTile(int x, int y)
{
    if (!isValidPosition(x, y))
        return nullptr;

    return m_tiles[y][x].get();
}

void TileMap::clear()
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_tiles[y][x].reset();
        }
    }
}

void TileMap::resize(int width, int height)
{
    m_width = width;
    m_height = height;

    m_tiles.resize(height);
    for (int y = 0; y < height; ++y)
    {
        m_tiles[y].resize(width);
        for (int x = 0; x < width; ++x)
        {
            m_tiles[y][x] = nullptr;
        }
    }
}

sf::Vector2i TileMap::getSize() const
{
    return sf::Vector2i(m_width, m_height);
}

float TileMap::getTileSize() const
{
    return m_tileSize;
}

void TileMap::setTileSize(float size)
{
    m_tileSize = size;

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            if (m_tiles[y][x])
                m_tiles[y][x]->setSize(size);
        }
    }
}