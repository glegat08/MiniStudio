#pragma once
#include "GameObject.h"
#include "HeroState.h"

class HeroState;

namespace HeroStateNames
{
	enum class stateName
	{
		idle,
		move,
		attack,
		dash,
		hurt,
		death
	};
}


class Hero : public IGameObject
{
public:
	Hero();
	~Hero() override = default;

	using stateName = HeroStateNames::stateName;

	int getHp() override;
	void getStat() override;

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
	HeroState m_stateManager;
};