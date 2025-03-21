#pragma once
#include "Projectile.h"

class Arrow : public IProjectile
{
public:
	Arrow(const std::string& name = "Arrow", int damage = 10);
	~Arrow() override;

	void initialize(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed) override;
	void update(const float& deltaTime) override;

	int getDamage() const override;
	bool isLifeTimePassed() const override;
};