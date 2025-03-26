#include "Game.h"

#include <iostream>
#include <random>

#include "AudioManager.h"
#include "Camera.h"
#include "Collision.h"
#include "Effect.h"
#include "Enemy.h"
#include "Map.h"
#include "TextureManager.h"
#include "UI.h"

Game* Game::m_gameInstance = nullptr;

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate, "Game")
	, m_deadPlayer(false)
{
	m_gameInstance = this;
	initialize();
}

void Game::initialize()
{
	TextureManager::getInstance().initialize();
	TextureManager::getInstance().loadAllGameTextures();

	SoundManager::getInstance().initialize();
	SoundManager::getInstance().loadAllGameSounds();
	SoundManager::getInstance().setMusicLoop(true);

	sf::FloatRect worldLimits(0, 0, 3000, 2500);

	Camera::getInstance().initialize(m_renderWindow);
	Camera::getInstance().setZoom(1.7f);
	Camera::getInstance().setWorldBounds(worldLimits);
	Camera::getInstance().setInterpolationSpeed(4.0f);

	WorldLimits::initialize(worldLimits);

	setMap();
	setLayer();
	setPlayer();
	setupHealthUI();
	setEnemy();
	setupScoreUI();
}

void Game::setPlayer()
{
	m_player = createPlayer();

	addCameraToPlayer();
	addHitboxToPlayer();

	m_gameObjects.push_back(m_player);
}

void Game::setEnemy()
{
	enemyGenerator(20);
}

std::shared_ptr<Hero> Game::createPlayer()
{
	auto player = std::make_shared<Hero>("Player");
	player->initialize(sf::Vector2f(1000.f, 1000.f), 50.f, sf::Color::Transparent, 300.f);
	return player;
}

void Game::addCameraToPlayer()
{
	auto cameraTarget = std::make_shared<CameraTarget>(1.0f, true);
	m_player->addComponent(cameraTarget);
}

void Game::addHitboxToPlayer()
{
	auto playerHitbox = static_cast<Hitbox*>(m_player->getComponent("Hitbox"));
	if (playerHitbox)
	{
		playerHitbox->setEnterCollision([this](Hitbox* player, Hitbox* other)
			{
				handlePlayerCollision(player, other);
			});
	}
}

void Game::handlePlayerCollision(Hitbox* player, Hitbox* other)
{
	auto enemy = dynamic_cast<IEnemy*>(other->getOwner());
	if (!enemy) 
		return;

	auto hero = static_cast<Hero*>(player->getOwner());
	if (!hero) 
		return;

	if (hero->getCurrentState() == stateName::death)
		return;

	if (enemy->isDead())
		return;

	if (hero->getCurrentState() == stateName::attack)
		handlePlayerAttackingEnemy(hero, enemy);
	else if (!enemy->isNotAttacking())
		handleEnemyAttackingPlayer(hero, enemy);
}

void Game::setMap()
{
	m_map = std::make_shared<TilesMap>("BaseMap", repr, 16);

	std::cout << "Map initialized with size: " << m_map->getWidth() << "x" << m_map->getHeight() << std::endl;
	std::cout << "Map has " << m_map->getLayerCount() << " layers" << std::endl;

	m_gameObjects.push_back(m_map);
}

void Game::setLayer()
{
	m_mapLayers = std::make_shared<TilesMap>("PathLayer", path, 32);

	m_mapLayers->setScale(2.0f, 2.0f);

	std::cout << "Pathlayer has been added with size: " << m_mapLayers->getWidth()
		<< "x" << m_mapLayers->getHeight() << std::endl;

	m_gameObjects.push_back(m_mapLayers);
}

void Game::handlePlayerAttackingEnemy(Hero* hero, IEnemy* enemy)
{
	auto player_render = static_cast<SquareRenderer*>(hero->getComponent("SquareRenderer"));
	if (!player_render)
		return;

	bool wasAlreadyDead = enemy->isDead();

	sf::Vector2f playerPos = player_render->getPosition();
	enemy->takeDamage(100, playerPos);
	enemy->knockBack(playerPos, 600.0f);

	if (!wasAlreadyDead && enemy->isDead())
	{
		auto meleeEnemy = dynamic_cast<MeleeEnemy*>(enemy);
		if (meleeEnemy)
			addScore(100);
		else
		{
			auto rangedEnemy = dynamic_cast<RangedEnemy*>(enemy);
			if (rangedEnemy)
				addScore(150);
			else
				addScore(50);
		}
	}
}

void Game::handleEnemyAttackingPlayer(Hero* hero, IEnemy* enemy)
{
	auto enemy_render = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
	if (!enemy_render)
		return;

	sf::Vector2f enemyPos = enemy_render->getPosition();
	hero->takeDamage(10, enemyPos);
	hero->knockBack(enemyPos, 500.0f);
}

void Game::processInput(const sf::Event& event)
{
	m_player->processInput(event);

	SceneBase::processInput(event);
}

void Game::update(const float& deltaTime)
{
	SceneBase::update(deltaTime);

	m_enemySpawnTimer += deltaTime;

	if (m_enemySpawnTimer >= m_enemySpawnInterval && getCurrentEnemyCount() < m_maxEnemyCount)
	{
		spawnEnemyWave(m_enemiesPerWave);
		m_enemySpawnTimer = 0.0f;
	}

	for (auto& gameObject : m_gameObjects)
	{
		if (gameObject)
			gameObject->update(deltaTime);
	}

	bool isPlayerDead = m_player->getCurrentState() == stateName::death;

	if (isPlayerDead && !m_deadPlayer)
		IEnemy::heroIsDead(m_gameObjects);
	else if (!isPlayerDead)
	{
		auto player_render = static_cast<SquareRenderer*>(m_player->getComponent("SquareRenderer"));
		if (player_render)
		{
			sf::Vector2f playerPos = player_render->getPosition();
			IEnemy::updateAllEnemyLOS(m_gameObjects, playerPos);
		}
	}

	m_deadPlayer = isPlayerDead;

	cleanupProjectiles();
	cleanupEffects();
	cleanupDeadEnemies();
	Hitbox::resolveCollisions(m_gameObjects);

	Camera::getInstance().update(deltaTime);
}

void Game::render()
{
	Camera::getInstance().apply();

	if (m_map)
		m_map->render(*m_renderWindow);
	if (m_mapLayers)
		m_mapLayers->render(*m_renderWindow);

	std::vector<std::shared_ptr<CompositeGameObject>> gameObjects;
	for (auto& game_object : m_gameObjects) 
	{
		if (!dynamic_cast<TilesMap*>(game_object.get()) && game_object->getCategory() != "UI") 
			gameObjects.push_back(game_object);
	}

	std::sort(gameObjects.begin(), gameObjects.end(),
		[](const std::shared_ptr<CompositeGameObject>& first, const std::shared_ptr<CompositeGameObject>& second)
		{
			auto first_enemy = dynamic_cast<IEnemy*>(first.get());
			auto second_enemy = dynamic_cast<IEnemy*>(second.get());

			if (first_enemy && first_enemy->isDead() && (!second_enemy || !second_enemy->isDead()))
				return true;

			if (second_enemy && second_enemy->isDead() && (!first_enemy || !first_enemy->isDead()))
				return false;

			auto first_square_render = static_cast<SquareRenderer*>(first->getComponent("SquareRenderer"));
			auto second_square_render = static_cast<SquareRenderer*>(second->getComponent("SquareRenderer"));

			if (first_square_render && second_square_render) 
			{
				float first_positionY = first_square_render->getPosition().y;
				float second_positionY = second_square_render->getPosition().y;
				return first_positionY < second_positionY;
			}

			return false;
		});

	for (auto& game_object : gameObjects) 
	{
		game_object->render(*m_renderWindow);
	}

	sf::View defaultView = m_renderWindow->getDefaultView();
	m_renderWindow->setView(defaultView);

	for (auto& game_object : m_gameObjects) 
	{
		if (game_object->getCategory() == "UI") 
			game_object->render(*m_renderWindow);
	}

	if (!m_player->isAlive()) 
	{
		m_isGameOver = true;
		displayGameOver();
	}

	SceneBase::render();
}

Game* Game::getInstance()
{
	return m_gameInstance;
}

std::shared_ptr<Arrow> Game::createArrow(const sf::Vector2f& pos, const sf::Vector2f& direction, int damage)
{
	auto arrow = std::make_shared<Arrow>("Arrow", 1);
	arrow->initialize(pos, direction, 400.f);

	m_gameObjects.push_back(arrow);
	return arrow;
}

void Game::enemyGenerator(int initialCount)
{
	spawnEnemyWave(initialCount);
}

void Game::cleanupProjectiles()
{
	auto iterator = m_gameObjects.begin();
	while (iterator != m_gameObjects.end())
	{
		if ((*iterator)->getCategory() == "Projectile")
		{
			auto projectile = dynamic_cast<IProjectile*>(iterator->get());
			if (projectile && projectile->isLifeTimePassed())
			{
				iterator = m_gameObjects.erase(iterator);
				continue;
			}
		}
		++iterator;
	}
}

void Game::addEffect(std::shared_ptr<CompositeGameObject> effect)
{
	m_gameObjects.push_back(effect);
}

void Game::cleanupEffects()
{
	auto iterator = m_gameObjects.begin();
	while (iterator != m_gameObjects.end())
	{
		if ((*iterator)->getCategory() == "Effect")
		{
			auto effect = dynamic_cast<BloodEffect*>(iterator->get());
			if (effect && effect->isFinished())
			{
				iterator = m_gameObjects.erase(iterator);
				continue;
			}
		}
		++iterator;
	}
}

void Game::setupHealthUI()
{
	auto m_healthUI = std::make_shared<HealthUI>("PlayerHealthUI");
	m_healthUI->initialize(m_player.get(), 0.25f);

	m_healthUI->setPosition(sf::Vector2f(100.f, 100.f));

	m_gameObjects.push_back(m_healthUI);
}

void Game::displayGameOver()
{
	sf::RectangleShape overlay(sf::Vector2f(m_renderWindow->getSize()));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));

	sf::Font gameOverFont;
	gameOverFont.loadFromFile("C:\\Users\\guill\\source\\repos\\MiniStudio\\resources\\font.ttf");

	m_gameOverText.setFont(gameOverFont);
	m_gameOverText.setString("GAME OVER");
	m_gameOverText.setCharacterSize(80);
	m_gameOverText.setFillColor(sf::Color::Red);

	sf::FloatRect textBounds = m_gameOverText.getLocalBounds();
	m_gameOverText.setPosition
	(
		(m_renderWindow->getSize().x - textBounds.width) / 2,
		(m_renderWindow->getSize().y - textBounds.height) / 2
	);

	m_renderWindow->draw(overlay);
	m_renderWindow->draw(m_gameOverText);
}

int Game::getCurrentEnemyCount() const
{
	int count = 0;
	for (const auto& game_object : m_gameObjects)
	{
		if (game_object && game_object->getCategory() == "Enemy" && !dynamic_cast<IEnemy*>(game_object.get())->isDead())
			count++;
	}
	return count;
}

void Game::spawnEnemyWave(int count)
{
	static int enemyIndex = 0;

	for (int idx = 0; idx < count; idx++)
	{
		sf::Vector2f spawnPos = getRandomSpawnPosition();
		std::shared_ptr<IEnemy> enemy = createRandomEnemy(spawnPos, ++enemyIndex);

		m_gameObjects.push_back(enemy);
	}
}

sf::Vector2f Game::getRandomSpawnPosition()
{
	static std::random_device random_device;
	static std::mt19937 gen(random_device());

	sf::Vector2f playerPos;
	auto player_renderer = static_cast<SquareRenderer*>(m_player->getComponent("SquareRenderer"));
	if (player_renderer)
		playerPos = player_renderer->getPosition();

	std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);
	float angle = angleDist(gen);

	std::uniform_real_distribution<float> distanceDist(m_minSpawnDistance, m_maxSpawnDistance);
	float distance = distanceDist(gen);

	sf::Vector2f spawnPos
	(
		playerPos.x + std::cos(angle) * distance,
		playerPos.y + std::sin(angle) * distance
	);

	sf::FloatRect worldBounds(0, 0, 3000, 2500);
	spawnPos.x = std::max(worldBounds.left, std::min(spawnPos.x, worldBounds.left + worldBounds.width));
	spawnPos.y = std::max(worldBounds.top, std::min(spawnPos.y, worldBounds.top + worldBounds.height));

	return spawnPos;
}

std::shared_ptr<IEnemy> Game::createRandomEnemy(const sf::Vector2f& position, int index)
{
	static std::random_device random_device;
	static std::mt19937 gen(random_device());
	std::uniform_int_distribution<> randomEnemy(0, 1); // 0 for melee, 1 for ranged

	int enemyType = randomEnemy(gen);
	std::shared_ptr<IEnemy> enemy;

	if (enemyType == 0)
	{
		auto meleeEnemy = std::make_shared<MeleeEnemy>("MeleeEnemy" + std::to_string(index));
		meleeEnemy->init(position);
		enemy = meleeEnemy;
	}
	else
	{
		auto rangedEnemy = std::make_shared<RangedEnemy>("RangedEnemy" + std::to_string(index));
		rangedEnemy->init(position);
		enemy = rangedEnemy;
	}

	return enemy;
}

void Game::cleanupDeadEnemies()
{
	sf::FloatRect visibleArea = Camera::getInstance().getVisibleArea();
	visibleArea.left -= 200.0f;
	visibleArea.top -= 200.0f;
	visibleArea.width += 400.0f;
	visibleArea.height += 400.0f;

	auto iterator = m_gameObjects.begin();
	while (iterator != m_gameObjects.end())
	{
		auto enemy = dynamic_cast<IEnemy*>(iterator->get());
		if (enemy && enemy->isDead())
		{
			auto renderer = static_cast<SquareRenderer*>((*iterator)->getComponent("SquareRenderer"));
			if (renderer)
			{
				sf::Vector2f pos = renderer->getPosition();
				if (!visibleArea.contains(pos))
				{
					iterator = m_gameObjects.erase(iterator);
					continue;
				}
			}
		}
		++iterator;
	}
}

void Game::addScore(int points)
{
	m_score += points;
}

void Game::setupScoreUI()
{
	m_scoreUI = std::make_shared<ScoreUI>("PlayerScoreUI");
	m_scoreUI->initialize(this, 0.25f);
	m_scoreUI->setPosition(sf::Vector2f(910.f, 50.f));
	m_gameObjects.push_back(m_scoreUI);
}