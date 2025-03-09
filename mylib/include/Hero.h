#pragma once
#include "GameObject.h"

class HeroState;

class Hero : public IGameObject
{
public:
	Hero();

	int getHp() override;
	void getStat() override;
	void processInput(const sf::Event& event) override;


private:
	int m_health;
	int m_armor;
	int m_strength;
	int m_mana;
	int m_stamina;

private:
	float m_speed;
	float m_velocity;
	bool m_isMovingUp;
	bool m_isMovingDown;
	bool m_isMovingLeft;	
	bool m_isMovingRight;

private:
	HeroState m_stateManager;
};