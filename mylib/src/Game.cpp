#include "Game.h"

#include <random>

#include "Camera.h"
#include "Collision.h"
#include "Enemy.h"
#include "TextureManager.h"

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate)
{
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
	setEnemy();
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
	enemyGenerator(100);
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

	if (enemy->isDead())
		return;

	if (hero->getCurrentState() == stateName::attack)
		handlePlayerAttackingEnemy(hero, enemy);
	else if (!enemy->isNotAttacking())
		handleEnemyAttackingPlayer(hero, enemy);
}

void Game::handlePlayerAttackingEnemy(Hero* hero, IEnemy* enemy)
{
	auto playerRenderer = static_cast<SquareRenderer*>(hero->getComponent("SquareRenderer"));
	if (!playerRenderer) 
		return;

	sf::Vector2f playerPos = playerRenderer->getPosition();
	enemy->takeDamage(100);
	enemy->knockBack(playerPos, 600.0f);
}

void Game::handleEnemyAttackingPlayer(Hero* hero, IEnemy* enemy)
{
	auto enemyRenderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
	if (!enemyRenderer)
		return;

	sf::Vector2f enemyPos = enemyRenderer->getPosition();
	hero->takeDamage(1);
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
		gameObject->update(deltaTime);
	}

	auto playerRender = static_cast<SquareRenderer*>(m_player->getComponent("SquareRenderer"));
	if (playerRender)
	{
		sf::Vector2f playerPos = playerRender->getPosition();
		IEnemy::updateAllEnemyLOS(m_gameObjects, playerPos);
	}

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
			auto rendererA = static_cast<SquareRenderer*>(a->getComponent("SquareRenderer"));
			auto rendererB = static_cast<SquareRenderer*>(b->getComponent("SquareRenderer"));

			if (!rendererA || !rendererB)
				return false;

			float posYA = rendererA->getPosition().y;
			float posYB = rendererB->getPosition().y;

			auto enemyA = dynamic_cast<IEnemy*>(a.get());
			auto enemyB = dynamic_cast<IEnemy*>(b.get());

			if (enemyA && enemyA->isDead() && (!enemyB || !enemyB->isDead()))
				return true; 

			if (enemyB && enemyB->isDead() && (!enemyA || !enemyA->isDead()))
				return false;

			return posYA < posYB;
		});

	for (auto& gameObject : sortedObjects)
	{
		gameObject->render(*m_renderWindow);
	}

	SceneBase::render();
}

void Game::enemyGenerator(int count)
{
	sf::Vector2u windowSize = m_renderWindow->getSize();

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> randomEnemy(0, 1); // 0 for melee, 1 for ranged
	std::uniform_real_distribution<float> positionX(windowSize.x * 0.1f, windowSize.x * 0.9f);
	std::uniform_real_distribution<float> positionY(windowSize.y * 0.1f, windowSize.y * 0.9f);

	for (int i = 0; i < count; i++)
	{
		sf::Vector2f randomPos(positionX(gen), positionY(gen));

		auto playerRenderer = static_cast<SquareRenderer*>(m_player->getComponent("SquareRenderer"));
		if (playerRenderer)
		{
			sf::Vector2f playerPos = playerRenderer->getPosition();
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
