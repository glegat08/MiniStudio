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
		mele_attack,
		range_attack,
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
	bool isMeleAttacking();
	bool isRangeAttacking();
	bool isInvulnerable();
	bool isIdle() const;

	void takeDamage(int damage);
	void setInvulnerable(float duration);
	void updateInvulnerabilityEffect();
	void meleAttacking();
	void rangeAttacking();
	void handleInput();
	void update(float deltaTime);
	void setSpeed(float speed);
	int getStrenght() const;
	int getHp() override;
	float getSpeed() const;
	void move(const sf::Vector2f& offset);
	void setIdle(bool idle);
	void pushState(stateName newState);
	void popState();

	sf::Texture& getTexture(const stateName& stateName_);
	sf::Sprite& getSprite();
	stateName getCurrentState() const;
	HeroState& getStateManager();

	void setStateTexture();

	sf::FloatRect getHitbox() const;
	sf::Vector2f getPlayerPosition();
	sf::Vector2f getPlayerCenter();

protected:
	int m_health;
	int m_armor;
	int m_strength;
	int m_mana;
	int m_stamina;

	float m_speed;
	float m_velocity;

	bool m_isIdle;
	bool m_isAttacking;
	bool m_isHurt = false;
	bool m_isDead = false;

protected:
	bool m_isInvulnerable = false;
	sf::Clock m_invulnerabilityTimer;
	float m_invulnerabilityDuration = 0.f;

	sf::Sprite m_sprites;
	std::map<stateName, sf::Texture> m_textures;
	HeroState m_stateManager;
	stateName m_currentStateName;
};