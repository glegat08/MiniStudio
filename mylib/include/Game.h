#pragma once
#include <iostream>
#include "Arrow.h"
#include "Collision.h"
#include "SceneBase.h"
#include "Hero.h"
#include "IEnemy.h"
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

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;

    static Game* getInstance();

    std::shared_ptr<Arrow> createArrow(const sf::Vector2f& pos, const sf::Vector2f& direction, int damage);
    void addEffect(std::shared_ptr<CompositeGameObject> effect);

private:
    void setPlayer();
    static std::shared_ptr<Hero> createPlayer();
    void addCameraToPlayer();
    void addHitboxToPlayer();
    void handlePlayerCollision(Hitbox* player, Hitbox* other);
    void handlePlayerAttackingEnemy(Hero* hero, IEnemy* enemy);
    void handleEnemyAttackingPlayer(Hero* hero, IEnemy* enemy);

    // Enemy-related methods
    void setEnemy();
    void enemyGenerator(int count);

    void cleanupProjectiles();
    void cleanupEffects();

    void setupHealthUI();

private:
	std::shared_ptr<Hero> m_player;
    bool m_deadPlayer;

	std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
    static Game* m_gameInstance;

	std::shared_ptr<TilesMap> m_map;
	std::shared_ptr<TilesMap> m_mapLayers;
	std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
};
