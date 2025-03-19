#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

class Hitbox : public Component
{
public:
    using CollisionEvents = std::function<void(Hitbox*, Hitbox*)>;

    Hitbox(const sf::Vector2f& size = sf::Vector2f(50.0f, 50.0f));
    ~Hitbox() override;

    void initialize() override;
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    bool checkCollision(const Hitbox* other) const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;

    void setOffset(const sf::Vector2f& offset);
    sf::Vector2f getOffset() const;

    void setEnterCollision(CollisionEvents events);
    void setExitCollision(CollisionEvents events);

    void enterCollision(Hitbox* other);
    void exitCollision(Hitbox* other);

    void setDebugDraw(bool debugDraw);

    static void resolveCollisions(const std::vector<std::shared_ptr<CompositeGameObject>>& gameObjects);

private:
    sf::Vector2f m_size;
    sf::Vector2f m_offset;

    bool m_debugDraw;
    sf::RectangleShape m_debugShape;

    std::vector<Hitbox*> m_currentCollisions;

    CollisionEvents m_enterCollision;
    CollisionEvents m_exitCollision;

    void getBounds(float& minX, float& minY, float& maxX, float& maxY) const;
};