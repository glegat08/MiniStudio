#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "TextureManager.h"

class Decoration : public GameObject
{
public:
    Decoration(const std::string& name, const std::string& texturePath, const sf::Vector2f& position);
    virtual ~Decoration() = default;

    void render(sf::RenderWindow& window) override;
    void setScale(float scaleX, float scaleY);
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    void setZOrder(int zOrder);
    int getZOrder() const;

private:
    sf::Sprite m_sprite;
    std::string m_texturePath;
    sf::Vector2f m_position;
    int m_zOrder;
};