#pragma once

#include "SceneBase.h"
#include "Hero.h"

class Game : public SceneBase
{
public:
	Game(sf::RenderWindow* window, const float& framerate);
	~Game() = default;

	void setPlayer();
	void setEnemy();
	void initialize();

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;

private:
	std::shared_ptr<Hero> m_player;
	std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
};