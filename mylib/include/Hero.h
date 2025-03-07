#pragma once
#include "GameObject.h"

class HeroState;

class Hero : public IGameObject
{
public:
	Hero();

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