#include "Collision.h"
#include "Composite.h"
#include <algorithm>
#include <iostream>

Hitbox::Hitbox(const sf::Vector2f& size)
	: Component("Hitbox")
	, m_size(size)
	, m_offset(0.0f, 0.0f)
	, m_debugDraw(false)
{
	m_debugShape.setSize(m_size);
	m_debugShape.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_debugShape.setFillColor(sf::Color::Transparent);
	m_debugShape.setOutlineColor(sf::Color::Red);
	m_debugShape.setOutlineThickness(1.0f);
}

Hitbox::~Hitbox()
{
}

void Hitbox::initialize()
{
	Component::initialize();
}

void Hitbox::update(const float& deltaTime)
{
	if (m_owner)
	{
		auto square_renderer = static_cast<SquareRenderer*>(m_owner->getComponent("SquareRenderer"));
		if (square_renderer)
		{
			sf::Vector2f pos = square_renderer->getPosition() + m_offset;
			m_debugShape.setPosition(pos);
		}
	}
}

void Hitbox::render(sf::RenderWindow& window)
{
	if (m_debugDraw)
		window.draw(m_debugShape);
}

bool Hitbox::checkCollision(const Hitbox* other) const
{
	float minAX, minAY, maxAX, maxAY;
	getBounds(minAX, minAY, maxAX, maxAY);

	float minBX, minBY, maxBX, maxBY;
	other->getBounds(minBX, minBY, maxBX, maxBY);

	return (minAX <= maxBX && maxAX >= minBX &&
		minAY <= maxBY && maxAY >= minBY);
}

void Hitbox::setSize(const sf::Vector2f& size)
{
	m_size = size;
	m_debugShape.setSize(m_size);
	m_debugShape.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
}

sf::Vector2f Hitbox::getSize() const
{
	return m_size;
}

void Hitbox::setOffset(const sf::Vector2f& offset)
{
	m_offset = offset;
}

sf::Vector2f Hitbox::getOffset() const
{
	return m_offset;
}

void Hitbox::setEnterCollision(CollisionEvents events)
{
	m_enterCollision = events;
}

void Hitbox::setExitCollision(CollisionEvents events)
{
	m_exitCollision = events;
}

void Hitbox::enterCollision(Hitbox* other)
{
	if (m_enterCollision)
		m_enterCollision(this, other);
}

void Hitbox::exitCollision(Hitbox* other)
{
	if (!other || !m_exitCollision)
		return;

	if (m_exitCollision)
		m_exitCollision(this, other);
}

void Hitbox::setDebugDraw(bool debugDraw)
{
	m_debugDraw = debugDraw;
}

void Hitbox::resolveCollisions(const std::vector<std::shared_ptr<CompositeGameObject>>& gameObjects)
{
    std::vector<Hitbox*> hitboxes;
    std::unordered_map<Hitbox*, bool> hitboxValidity;

    for (const auto& gameObject : gameObjects)
    {
        if (!gameObject) continue;

        auto hitbox = static_cast<Hitbox*>(gameObject->getComponent("Hitbox"));
        if (hitbox)
        {
            hitboxes.push_back(hitbox);
            hitboxValidity[hitbox] = true;

            for (auto iterator = hitbox->m_currentCollisions.begin(); iterator != hitbox->m_currentCollisions.end();)
            {
                Hitbox* target = *iterator;
                if (!target || hitboxValidity.find(target) == hitboxValidity.end())
                    iterator = hitbox->m_currentCollisions.erase(iterator);
                else
                    ++iterator;
            }
        }
    }

    for (size_t idx = 0; idx < hitboxes.size(); ++idx)
    {
        Hitbox* hitboxA = hitboxes[idx];
        std::vector<Hitbox*> currentCollisions;

        for (size_t jdx = idx + 1; jdx < hitboxes.size(); ++jdx)
        {
            Hitbox* hitboxB = hitboxes[jdx];

            if (hitboxA->checkCollision(hitboxB))
            {
                currentCollisions.push_back(hitboxB);

                auto iteratorA = std::find(hitboxA->m_currentCollisions.begin(),
                    hitboxA->m_currentCollisions.end(), hitboxB);

                if (iteratorA == hitboxA->m_currentCollisions.end())
                {
                    if (hitboxA->m_enterCollision)
                    {
                        try
                        {
                            hitboxA->enterCollision(hitboxB);
                        }
                        catch (...)
                        {
                            hitboxA->m_enterCollision = nullptr;
                            std::cerr << "Error in enterCollision" << std::endl;
                        }
                    }

                    if (hitboxB->m_enterCollision)
                    {
                        try 
                        {
                            hitboxB->enterCollision(hitboxA);
                        }
                        catch (...) 
                        {
                            hitboxB->m_enterCollision = nullptr;
                            std::cerr << "Error in enterCollision" << std::endl;
                        }
                    }
                }
            }
        }

        for (auto iterator = hitboxA->m_currentCollisions.begin(); iterator != hitboxA->m_currentCollisions.end();)
        {
            Hitbox* finishedCollision = *iterator;

            if (std::find(currentCollisions.begin(), currentCollisions.end(), finishedCollision) == currentCollisions.end())
            {
                if (hitboxA->m_exitCollision)
                {
                    try 
                    {
                        hitboxA->exitCollision(finishedCollision);
                    }
                    catch (...) 
                    {
                        hitboxA->m_exitCollision = nullptr;
                        std::cerr << "Error in exitCollision" << std::endl;
                    }
                }

                if (hitboxValidity.find(finishedCollision) != hitboxValidity.end() && finishedCollision->m_exitCollision)
                {
                    try 
                    {
                        finishedCollision->exitCollision(hitboxA);
                    }
                    catch (...) 
                    {
                        finishedCollision->m_exitCollision = nullptr;
                        std::cerr << "Error in exitCollision" << std::endl;
                    }
                }

                iterator = hitboxA->m_currentCollisions.erase(iterator);
            }
            else
                ++iterator;
        }

        hitboxA->m_currentCollisions = currentCollisions;
    }
}

void Hitbox::getBounds(float& minX, float& minY, float& maxX, float& maxY) const
{
	auto square_renderer = static_cast<SquareRenderer*>(m_owner->getComponent("SquareRenderer"));
	if (!square_renderer)
	{
		minX = minY = maxX = maxY = 0.0f;
		return;
	}

	sf::Vector2f pos = square_renderer->getPosition() + m_offset;

	minX = pos.x - m_size.x / 2.0f;
	minY = pos.y - m_size.y / 2.0f;
	maxX = pos.x + m_size.x / 2.0f;
	maxY = pos.y + m_size.y / 2.0f;
}

