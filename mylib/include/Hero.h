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

	bool isAlive();
	bool isShooting();
	bool isAttacking();
	bool isInvulnerable();
	bool isIdle() const;

	void takeDamage(int damage);
	void setInvulnerable(float duration);
	void updateInvulnerabilityEffect();
	void attacking();
	void handleInput();
	void update(float deltaTime);
	void setSpeed(float speed);
	float getSpeed() const;
	void move(const sf::Vector2f& offset);
	void setIdle(bool idle);
	void pushState(stateName newState);
	void popState();

	int getHp() override;
	void getStat() override;

	sf::FloatRect getHitbox() const;
	sf::Vector2f getPlayerPosition();
	sf::Vector2f getPlayerCenter();

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	sf::FloatRect getGlobalBounds() const;

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