#include "Projectile.h"

#include "Animation.h"
#include "Hero.h"

IProjectile::IProjectile(const std::string& name)
	: CompositeGameObject(name)
	, m_speed(300.f)
	, m_damage(10)
	, m_direction(0.f, 0.f)
	, m_TimeBeforeDestroy(0.f)
	, m_maxTimeBeforeDestroy(3.0f)
{
	setCategory("Projectile");
}

IProjectile::~IProjectile()
{
}

void IProjectile::initialize(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed)
{
}

void IProjectile::update(const float& deltaTime)
{
	m_TimeBeforeDestroy += deltaTime;

	CompositeGameObject::update(deltaTime);
}

int IProjectile::getDamage() const
{
	return m_damage;
}

bool IProjectile::isLifeTimePassed() const
{
	return m_TimeBeforeDestroy >= m_maxTimeBeforeDestroy;
}

void IProjectile::updateAnimation()
{
	auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
	auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

	if (square_renderer && animation_component)
	{
		sf::Vector2f position = square_renderer->getPosition();
		animation_component->updatePosition(position);
	}
}

void IProjectile::updateVisualDirection()
{
}
