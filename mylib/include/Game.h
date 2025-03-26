#pragma once
#include "Arrow.h"
#include "SceneBase.h"
#include "Hero.h"
#include "Tiles.h"

class IEnemy;
class Hitbox;
class ScoreUI;

class Game : public SceneBase
{
public:
    Game(sf::RenderWindow* window, const float& framerate);
    ~Game() = default;

    void processInput(const sf::Event& event) override;
    void update(const float& deltaTime) override;
    void render() override;

    static Game* getInstance();

    std::shared_ptr<Arrow> createArrow(const sf::Vector2f& pos, const sf::Vector2f& direction, int damage);
    void addEffect(std::shared_ptr<CompositeGameObject> effect);

    int getScore() const { return m_score; }
    void addScore(int points);
    void resetScore() { m_score = 0; }

private:
    void initialize();

    void setPlayer();
    static std::shared_ptr<Hero> createPlayer();
    void addCameraToPlayer();
    void addHitboxToPlayer();
    void handlePlayerCollision(Hitbox* player, Hitbox* other);
    void handlePlayerAttackingEnemy(Hero* hero, IEnemy* enemy);
    void handleEnemyAttackingPlayer(Hero* hero, IEnemy* enemy);

private:
    void setEnemy();
    void enemyGenerator(int initialCount);

    float m_enemySpawnTimer = 0.0f;
    float m_enemySpawnInterval = 3.0f;
    int m_maxEnemyCount = 100;
    int m_enemiesPerWave = 3;
    float m_minSpawnDistance = 400.0f;
    float m_maxSpawnDistance = 800.0f;

    void spawnEnemyWave(int count);
    sf::Vector2f getRandomSpawnPosition();
    std::shared_ptr<IEnemy> createRandomEnemy(const sf::Vector2f& position, int index);
    int getCurrentEnemyCount() const;
    void cleanupDeadEnemies();

private:
    void cleanupProjectiles();
    void cleanupEffects();

    void setupHealthUI();
    void setupScoreUI();

    void setMap();
    void setLayer();

private:
    std::shared_ptr<Hero> m_player;
    bool m_deadPlayer;

    std::vector<std::shared_ptr<CompositeGameObject>> m_gameObjects;
    static Game* m_gameInstance;

    std::shared_ptr<TilesMap> m_map;
    std::shared_ptr<TilesMap> m_mapLayers;

    int m_score = 0;
    std::shared_ptr<ScoreUI> m_scoreUI;

    void displayGameOver();
    sf::Text m_gameOverText;
    sf::Text m_winText;
    bool m_isGameOver = false;
};