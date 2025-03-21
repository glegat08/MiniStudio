#include "Game.h"
#include "Camera.h"
#include "Enemy.h"
#include "Map.h"
#include "TextureManager.h"

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate)
{
	initialize();
}

void Game::setPlayer()
{
	TextureManager::getInstance().initialize();

	m_player = std::make_shared<Hero>("Player");
	m_player->initialize(sf::Vector2f(400.f, 300.f), 50.f, sf::Color::Transparent, 600.f);

	auto cameraTarget = std::make_shared<CameraTarget>(1.0f, true);
	m_player->addComponent(cameraTarget);

	m_gameObjects.push_back(m_player);
}

void Game::setEnemy()
{
	sf::Vector2u windowSize = m_renderWindow->getSize();

	auto meleeEnemy = std::make_shared<MeleeEnemy>("MeleeEnemy1");
	sf::Vector2f meleePos(windowSize.x * 0.75f, windowSize.y * 0.25f);
	meleeEnemy->init(meleePos);
	m_gameObjects.push_back(meleeEnemy);

	auto meleeEnemy2 = std::make_shared<MeleeEnemy>("MeleeEnemy2");
	sf::Vector2f meleePos2(windowSize.x * 0.25f, windowSize.y * 0.75f);
	meleeEnemy2->init(meleePos2);
	m_gameObjects.push_back(meleeEnemy2);

	auto rangedEnemy = std::make_shared<RangedEnemy>("RangedEnemy1");
	sf::Vector2f rangedPos(windowSize.x * 0.75f, windowSize.y * 0.75f);
	rangedEnemy->init(rangedPos);
	m_gameObjects.push_back(rangedEnemy);

	auto rangedEnemy2 = std::make_shared<RangedEnemy>("RangedEnemy2");
	sf::Vector2f rangedPos2(windowSize.x * 0.25f, windowSize.y * 0.25f);
	rangedEnemy2->init(rangedPos2);
	m_gameObjects.push_back(rangedEnemy2);
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

void Game::initialize()
{
	Camera::getInstance().initialize(m_renderWindow);

	Camera::getInstance().setZoom(1.f);
	//Camera::getInstance().setWorldBounds(sf::FloatRect(0, 0, 3000, 3000));
	Camera::getInstance().setInterpolationSpeed(4.0f);

	setMap();
	setLayer();
	setPlayer();
	setEnemy();
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

	Camera::getInstance().update(deltaTime);
}

void Game::render()
{
	Camera::getInstance().apply();

	/*if (m_map)
	{
		m_map->render(*m_renderWindow);
	}

	if (m_mapLayers)
	{
		m_mapLayers->render(*m_renderWindow);
	}*/

	for (auto& gameObject : m_gameObjects)
	{
		gameObject->render(*m_renderWindow);
	}

	SceneBase::render();
}
