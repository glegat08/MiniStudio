#pragma once
#include <SFML/Graphics.hpp>

class WorldLimits
{
public:
    static void initialize(const sf::FloatRect& bounds)
    {
        m_world_bounds = bounds;
    }

    static sf::FloatRect getBounds()
    {
        return m_world_bounds;
    }

    static bool isInLimits(const sf::Vector2f& position)
    {
        return (position.x >= m_world_bounds.left &&
            position.x <= m_world_bounds.left + m_world_bounds.width &&
            position.y >= m_world_bounds.top &&
            position.y <= m_world_bounds.top + m_world_bounds.height);
    }

    static sf::Vector2f limits(const sf::Vector2f& position, float entityRadius = 0.0f)
    {
        sf::Vector2f result = position;

        if (result.x - entityRadius < m_world_bounds.left)
            result.x = m_world_bounds.left + entityRadius;
        else if (result.x + entityRadius > m_world_bounds.left + m_world_bounds.width)
            result.x = m_world_bounds.left + m_world_bounds.width - entityRadius;

        if (result.y - entityRadius < m_world_bounds.top)
            result.y = m_world_bounds.top + entityRadius;
        else if (result.y + entityRadius > m_world_bounds.top + m_world_bounds.height)
            result.y = m_world_bounds.top + m_world_bounds.height - entityRadius;

        return result;
    }

private:
    static sf::FloatRect m_world_bounds;
};