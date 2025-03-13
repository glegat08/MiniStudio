#pragma once
#include "Composite.h"
#include "Component.h"

class HeroState;

class Hero : public CompositeGameObject
{
public:
	Hero(const std::string& name = "Hero");
	~Hero() = default;

	void initialize(const sf::Vector2f& position, const float& size,
					const sf::Color& color, const float& speed);

private:
	int m_health;
	int m_armor;
	int m_strength;
	int m_mana;
	int m_stamina;

private:
	float m_speed;
	float m_velocity;

private:
	//HeroState m_stateManager;
};