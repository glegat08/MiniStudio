#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

class IGameObject
{
public:
	virtual ~IGameObject() = default;

	virtual void processInput(const sf::Event& event) = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

	virtual void addChild(std::shared_ptr<IGameObject> child) = 0;
	virtual void removeChild(const std::string& name) = 0;
	virtual IGameObject* getChild(const std::string& name) = 0;

	virtual void setName(const std::string& name) = 0;
	virtual std::string getName() const = 0;
};