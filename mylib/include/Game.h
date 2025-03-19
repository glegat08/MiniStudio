#pragma once

#include "Collision.h"
#include "SceneBase.h"
#include "Hero.h"
#include "IEnemy.h"

class Game : public SceneBase
{
public:
	Game(sf::RenderWindow* window, const float& framerate);
	~Game() = default;

	void initialize();

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;

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
private:
	std::shared_ptr<Hero> m_player;
	std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
};
