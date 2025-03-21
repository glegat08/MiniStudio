#include "Game.h"

#include <iostream>
#include <random>
#include "Camera.h"
#include "Collision.h"
#include "Effect.h"
#include "Enemy.h"
#include "Map.h"
#include "TextureManager.h"
#include "UI.h"

Game* Game::m_gameInstance = nullptr;

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate)
	, m_deadPlayer(false)
{
	m_gameInstance = this;
	initialize();
}

void Game::initialize()
{
	TextureManager::getInstance().initialize();
	TextureManager::getInstance().loadAllGameTextures();

	Camera::getInstance().initialize(m_renderWindow);
	Camera::getInstance().setZoom(1.f);
	Camera::getInstance().setInterpolationSpeed(4.0f);

	setPlayer();
	setupHealthUI();
	setEnemy();
  setMap();
  setLayer();
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
	enemyGenerator(10);
}

std::shared_ptr<Hero> Game::createPlayer()
{
	auto player = std::make_shared<Hero>("Player");
	player->initialize(sf::Vector2f(400.f, 300.f), 50.f, sf::Color::Transparent, 300.f);
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

	m_mapLayers->setScale(4.0f, 4.0f);

	std::cout << "Pathlayer has been added with size: " << m_mapLayers->getWidth()
		<< "x" << m_mapLayers->getHeight() << std::endl;

	m_gameObjects.push_back(m_mapLayers);
}

void Game::handlePlayerAttackingEnemy(Hero* hero, IEnemy* enemy)
{
	auto player_render = static_cast<SquareRenderer*>(hero->getComponent("SquareRenderer"));
	if (!player_render)
		return;

	sf::Vector2f playerPos = player_render->getPosition();
	enemy->takeDamage(25, playerPos);
	enemy->knockBack(playerPos, 600.0f);
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
	Hitbox::resolveCollisions(m_gameObjects);

	Camera::getInstance().update(deltaTime);
}

void Game::render()
{
	Camera::getInstance().apply();

	std::vector<std::shared_ptr<CompositeGameObject>> sortedObjects = m_gameObjects;

	std::sort(sortedObjects.begin(), sortedObjects.end(),
		[](const std::shared_ptr<CompositeGameObject>& a, const std::shared_ptr<CompositeGameObject>& b)
		{
			if (a->getCategory() == "UI")
				return false; // UI always rendered last
			if (b->getCategory() == "UI")
				return true;

			auto first_render = static_cast<SquareRenderer*>(a->getComponent("SquareRenderer"));
			auto second_render = static_cast<SquareRenderer*>(b->getComponent("SquareRenderer"));

			if (!first_render || !second_render)
				return false;

			float posYA = first_render->getPosition().y;
			float posYB = second_render->getPosition().y;

			auto enemyA = dynamic_cast<IEnemy*>(a.get());
			auto enemyB = dynamic_cast<IEnemy*>(b.get());

			if (enemyA && enemyA->isDead() && (!enemyB || !enemyB->isDead()))
				return true;

			if (enemyB && enemyB->isDead() && (!enemyA || !enemyA->isDead()))
				return false;

			return posYA < posYB;
		});

	// First render all non-UI game objects with the game camera
	for (auto& gameObject : sortedObjects)
	{
		if (gameObject->getCategory() != "UI")
			gameObject->render(*m_renderWindow);
	}

	// Set back to default view for UI elements
	sf::View defaultView = m_renderWindow->getDefaultView();
	m_renderWindow->setView(defaultView);

	// Then render all UI elements with the default camera
	for (auto& gameObject : sortedObjects)
	{
		if (gameObject->getCategory() == "UI")
			gameObject->render(*m_renderWindow);
	}

	SceneBase::render();
}

Game* Game::getInstance()
{
	return m_gameInstance;
}

std::shared_ptr<Arrow> Game::createArrow(const sf::Vector2f& pos, const sf::Vector2f& direction, int damage)
{
	auto arrow = std::make_shared<Arrow>("Arrow", damage);
	arrow->initialize(pos, direction, 400.f);

	m_gameObjects.push_back(arrow);
	return arrow;
}

void Game::enemyGenerator(int count)
{
	sf::Vector2u windowSize = m_renderWindow->getSize();

	std::random_device random_device;
	std::mt19937 gen(random_device());

	std::uniform_int_distribution<> randomEnemy(0, 1); // 0 for melee, 1 for ranged
	std::uniform_real_distribution<float> positionX(windowSize.x * 0.1f, windowSize.x * 0.9f);
	std::uniform_real_distribution<float> positionY(windowSize.y * 0.1f, windowSize.y * 0.9f);

	for (int i = 0; i < count; i++)
	{
		sf::Vector2f randomPos(positionX(gen), positionY(gen));

		auto player_renderer = static_cast<SquareRenderer*>(m_player->getComponent("SquareRenderer"));
		if (player_renderer)
		{
			sf::Vector2f playerPos = player_renderer->getPosition();
			sf::Vector2f direction = randomPos - playerPos;
			float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

			if (distance < 150.0f)
			{
				if (distance > 0.1f) 
				{
					direction /= distance;
					randomPos = playerPos + direction * 150.0f;
				}
				else
				{
					randomPos = playerPos + sf::Vector2f(150.0f, 0.0f);
				}
			}
		}

		int enemyType = randomEnemy(gen);
		std::shared_ptr<IEnemy> enemy;

		if (enemyType == 0)
		{
			auto meleeEnemy = std::make_shared<MeleeEnemy>("MeleeEnemy" + std::to_string(i + 1));
			meleeEnemy->init(randomPos);
			enemy = meleeEnemy;
		}
		else
		{
			auto rangedEnemy = std::make_shared<RangedEnemy>("RangedEnemy" + std::to_string(i + 1));
			rangedEnemy->init(randomPos);
			enemy = rangedEnemy;
		}

		m_gameObjects.push_back(enemy);
	}
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