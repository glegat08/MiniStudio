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
	sf::Vector2f calculateMovementDirection();

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
	float m_frameTime;
	int m_currentFrame;
	int m_frameCount;
	const int m_frameWidth;
	const int m_frameHeight;
};

class Movement : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:

};

class Attack : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	float m_frameTime;
	int m_currentFrame;
	int m_frameCount;
	const int m_frameWidth;
	const int m_frameHeight;
	const float m_attackDuration;
};

class Dash : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime;
	int m_currentFrame;
	const int m_frameCount;
	const int m_frameWidth;
	const int m_frameHeight;
	const float m_dashSpeed;
	const float m_dashDuration;
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
	const float m_frameTime;
	int m_currentFrame;
	const int m_frameCount;
	const int m_frameWidth;
	const int m_frameHeight;
	const float m_hurtDuration;
};

class Death : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:
	sf::Clock m_elapsedTime;
	const float m_frameTime;
	int m_currentFrame;
	const int m_frameCount;
	const int m_frameWidth;
	const int m_frameHeight;
	const float m_deathDuration;

	bool m_animationComplete = false;
};