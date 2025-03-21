#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class CompositeGameObject;
class CameraTargetAdapter;

class Component
{
public:
	Component(const std::string& name = "Component");
	virtual ~Component() = default;

	virtual void initialize();
	virtual void processInput(const sf::Event& event);
	virtual void update(const float& deltaTime);
	virtual void render(sf::RenderWindow& window);

	void setOwner(CompositeGameObject* owner);
	CompositeGameObject* getOwner() const;

	std::string getName() const;
	void setName(const std::string& name);

protected:
	CompositeGameObject* m_owner;
	std::string m_name;
};

class SquareRenderer : public Component
{
public:
	SquareRenderer(float size, const sf::Color& color);
	~SquareRenderer() = default;

	void initialize() override;
	void render(sf::RenderWindow& window) override;

	void setSize(float size);
	void setColor(const sf::Color& color);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

private:
	sf::RectangleShape m_shape;
	float m_size;
	sf::Color m_color;
};

class PlayerController : public Component
{
public:
	PlayerController(float speed = 200.0f);
	~PlayerController() = default;

	void initialize() override;
	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;

	void setSpeed(float speed);
	float getSpeed() const;

	bool isMoving() const;
	bool isMovingUp() const;
	bool isMovingDown() const;
	bool isMovingLeft() const;
	bool isMovingRight() const;

	friend class CameraTargetAdapter;
	friend class AttackState;

private:
	float m_speed;
	sf::Vector2f m_direction;
	sf::Vector2f m_velocity;
	bool m_isMovingUp;
	bool m_isMovingDown;
	bool m_isMovingLeft;
	bool m_isMovingRight;
};