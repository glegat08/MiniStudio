#include "Decoration.h"

Decoration::Decoration(const std::string& name, const std::string& texturePath, const sf::Vector2f& position)
    : GameObject(name), m_texturePath(texturePath), m_position(position), m_zOrder(0)
{
    TextureManager& texManager = TextureManager::getInstance();
    
    if (!texManager.hasTexture(texturePath))
    {
        texManager.loadTexture(texturePath, texturePath);
    }
    
    m_sprite.setTexture(*texManager.getTexture(texturePath));
    m_sprite.setPosition(m_position);
}

void Decoration::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
    
    GameObject::render(window);
}

void Decoration::setScale(float scaleX, float scaleY)
{
    m_sprite.setScale(scaleX, scaleY);
}

void Decoration::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_sprite.setPosition(m_position);
}

sf::Vector2f Decoration::getPosition() const
{
    return m_position;
}

void Decoration::setZOrder(int zOrder)
{
    m_zOrder = zOrder;
}

int Decoration::getZOrder() const
{
    return m_zOrder;
}