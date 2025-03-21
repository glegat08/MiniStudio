#pragma once

#include "Composite.h"

class IProjectile : public CompositeGameObject
{
public:
	IProjectile(const std::string& name = "Projectile");
	virtual ~IProjectile();

	virtual void initialize(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed);
	void update(const float& deltaTime) override;

	virtual int getDamage() const = 0;
	virtual bool isLifeTimePassed() const = 0;

	void updateAnimation();
	void updateVisualDirection();

protected:
	float m_speed;
	int m_damage;
	sf::Vector2f m_direction;
	float m_TimeBeforeDestroy;
	float m_maxTimeBeforeDestroy;
};