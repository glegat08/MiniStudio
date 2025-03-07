#pragma once

#include "SceneBase.h"

class Game : public SceneBase
{
public:
	Game(sf::RenderWindow* window, const float& framerate);
	~Game() = default;

	void setPlayer();
	void setEnemy();

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;
};