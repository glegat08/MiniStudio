#pragma once

class Hero;

class IState
{
public:
	virtual ~IState() = default;
	virtual void handleInput(Hero& hero) = 0;
	virtual void update(Hero& hero, float deltaTime) = 0;
	virtual void setTexture(Hero& hero) = 0;
	virtual bool isTemporaryState() const;
};

class Idle : public IState
{
public:
	void isDoingNothing(const sf::Event& event);

private:
	bool m_isDoingNothing;
};

class Movement : public IState
{
public:
	void isMoving(const sf::Event& event);

private:
	bool m_isMovingUp;
	bool m_isMovingDown;
	bool m_isMovingLeft;
	bool m_isMovingRight;
};

class Attack : public IState
{
public:
	void isAttacking(const sf::Event& event);

private:
	bool m_isMeleAttack;
	bool m_isRangeAttack;
};

class Block : public IState
{
public:
	void isBlocking(const sf::Event& event);

private:
	bool m_isBlocking;
};

class Dash : public IState
{
public:
	void isDashing(const sf::Event& event);

private:
	bool m_isDashing;
};

class Hurt : public IState
{
public:
	void isTackingDamage(const sf::Event& event);

private:
	bool m_isTackingDamage;
};

class Death : public IState
{
public:
	void isDead(const sf::Event& event);

private:
	bool m_isDead;
};