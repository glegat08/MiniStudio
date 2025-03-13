#include "Game.h"
#include "Camera.h"

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate)
{
	initialize();
}

void Game::setPlayer()
{
	m_player = std::make_shared<Hero>("Player");
	m_player->setCategory("Player");
	m_player->addTag("Hero");
	m_player->initialize(sf::Vector2f(400.f, 300.f), 50.f, sf::Color::Blue, 1000.f);

	auto cameraTarget = std::make_shared<CameraTarget>(1.0f, true);
	m_player->addComponent(cameraTarget);

	m_gameObjects.push_back(m_player);
}

void Game::setEnemy()
{
	auto enemy = std::make_shared<CompositeGameObject>("Enemy");
	enemy->setCategory("Enemy");
	enemy->addTag("BasicEnemy");

	auto renderer = std::make_shared<SquareRenderer>(100.0f, sf::Color::Red);
	enemy->addComponent(renderer);

	sf::Vector2u windowSize = m_renderWindow->getSize();
	sf::Vector2f position(windowSize.x * 0.75f, windowSize.y * 0.75f);
	renderer->setPosition(position);

	m_gameObjects.push_back(enemy);

	auto enemy2 = std::make_shared<CompositeGameObject>("Enemy2");
	enemy2->setCategory("Enemy");
	enemy2->addTag("MageEnemy");

	auto render2 = std::make_shared<SquareRenderer>(85.f, sf::Color::Green);
	enemy2->addComponent(render2);

	sf::Vector2f position2(windowSize.x * 0.85f, windowSize.y * 0.85f);
	render2->setPosition(position2);

	m_gameObjects.push_back(enemy2);
}

void Game::initialize()
{
	Camera::getInstance().initialize(m_renderWindow);

	Camera::getInstance().setZoom(1.5f);
	Camera::getInstance().setWorldBounds(sf::FloatRect(0, 0, 3000, 3000));
	Camera::getInstance().setInterpolationSpeed(4.0f);

	setPlayer();
	setEnemy();
}

void Game::processInput(const sf::Event& event)
{
	if (m_player)
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

	Camera::getInstance().update(deltaTime);
}

void Game::render()
{
	Camera::getInstance().apply();

	for (auto& gameObject : m_gameObjects)
	{
		gameObject->render(*m_renderWindow);
	}

	SceneBase::render();
}
