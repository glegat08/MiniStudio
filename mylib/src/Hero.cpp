#include "Hero.h"

Hero::Hero(const std::string& name)
	: CompositeGameObject(name)
{
	setCategory("Player");
	addTag("Hero");
}

void Hero::initialize(const sf::Vector2f& position, const float& size, const sf::Color& color, const float& speed)
{
	auto renderer = std::make_shared<SquareRenderer>(size, color);
	this->addComponent(renderer);
	renderer->setPosition(position);
	auto controller = std::make_shared<PlayerController>(speed);
	addComponent(controller);
}

