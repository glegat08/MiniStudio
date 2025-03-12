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

};

class Dash : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:

};

class Hurt : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:

};

class Death : public IState
{
public:
	void handleInput(Hero& hero) override;
	void update(Hero& hero, float deltaTime) override;
	void setTexture(Hero& hero) override;

private:

};