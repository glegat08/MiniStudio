#pragma once
#include <iostream>

#include "Decoration.h"
#include "Hero.h"
#include "SceneBase.h"
#include "Map.h"
#include "Tiles.h"

class Game : public SceneBase
{
public:
	Game(sf::RenderWindow* window, const float& framerate);
	~Game() = default;

	void setPlayer();
	void setEnemy();
	void setMap();
	void setLayer();
	void initialize();

	void addDecoration(const std::string& name, const std::string& texturePath, const sf::Vector2f& position, float scale = 1.0f, int zOrder = 0);
	void removeDecoration(const std::string& name);
	Decoration* getDecoration(const std::string& name);

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;

private:
	std::shared_ptr<Hero> m_player;
	std::shared_ptr<TilesMap> m_map;
	std::shared_ptr<TilesMap> m_mapLayers;
	std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
	std::vector<std::shared_ptr<Decoration>> m_decorations;
};
