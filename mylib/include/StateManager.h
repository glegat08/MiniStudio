#pragma once
#include <SFML/Graphics.hpp>

class Hero;

class IState
{
public:
	virtual ~IState() = default;
	virtual void handleInput(Hero& hero) = 0;
	virtual void update(Hero& hero, float deltaTime) = 0;
	virtual void setTexture(Hero& hero) = 0;
	virtual bool isTemporaryState() const;

protected:
	static bool isGoingUp();
	static bool isGoingDown();
	static bool isGoingLeft();
	static bool isGoingRight();
	static bool isDashing();
	static bool canDash();
	static bool isMeleAttacking();
	static bool isRangeAttacking();

	static void updateDirection(Hero& hero);

	static bool m_dashAvailable;
	static sf::Clock m_dashCooldownClock;
	static const float m_dashCooldownDuration;

	static bool m_mouseRightPressed;
	static bool m_mouseLeftPressed;
};

class Idle : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	float m_frameTime = 0.5f;
	int m_currentFrame = 1;
	int m_frameCount = 2;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
};

class Movement : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;
	sf::Vector2f calculateMovementDirection();

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime = 0.2f;
	int m_currentFrame = 3;
	const int m_frameCount = 2;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
};

class MeleAttack : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	float m_frameTime = 0.1f;
	int m_currentFrame = 9;
	int m_frameCount = 6;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
	const float m_meleAttackDuration = 0.6f;
};

class RangeAttack : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	float m_frameTime = 0.1f;
	int m_currentFrame = 5;
	int m_frameCount = 4;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
	const float m_rangeAttackDuration = 0.4f;
};

class Dash : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;
	sf::Vector2f calculateMovementDirection();

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime = 1.f;
	int m_currentFrame = 2;
	const int m_frameCount = 1;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
	const float m_dashSpeed = 250.f;
	const float m_dashDuration = 1.f;
	sf::Vector2f m_dashDirection;
};

class Hurt : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime = 0.5f;
	int m_currentFrame = 60;
	const int m_frameCount = 1;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
	const float m_hurtDuration = 0.5f;
};

class Death : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime = 0.75f;
	int m_currentFrame = 43;
	const int m_frameCount = 18;
	const int m_frameWidth = 32;
	const int m_frameHeight = 32;
	const float m_deathDuration = 2.f;

	bool m_animationComplete = false;
};