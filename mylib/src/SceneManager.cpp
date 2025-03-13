#include "SceneManager.h"
#include "Game.h"
#include <iostream>

enum SceneStat
{
    MENU
    , GAME
    , CREDIT
    , PAUSE
};

SceneManager::SceneManager(const int& width, const int& height, const std::string& title)
    : m_window(std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title, sf::Style::Fullscreen))
{
	m_rootScene = std::make_unique<SceneBase>(m_window.get(), 60.f, "Root");

	auto gameScene = std::make_unique<Game>(m_window.get(), 60.f);

	m_currentScene = gameScene.get();

	m_rootScene->addChild(std::move(gameScene));
}

void SceneManager::addScene(std::unique_ptr<SceneBase> scene)
{
	m_rootScene->addChild(std::move(scene));
}

sf::RenderWindow* SceneManager::getWindow()
{
    return m_window.get();
}

SceneBase* SceneManager::getCurrentScene()
{
    return m_currentScene;
}

void SceneManager::setCurrentScene(const std::string& name)
{
    SceneBase* scene = m_rootScene->getChild(name);
    if (scene)
		m_currentScene = scene;
}

void SceneManager::processInput()
{
    // input
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window->close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
				if (m_currentScene->getName() == "Game")
					setCurrentScene("Pause");
				else if (m_currentScene->getName() == "Pause")
					setCurrentScene("Game");
            }

            if (event.key.code == sf::Keyboard::Escape)
                m_window->close();
        }

        m_currentScene->processInput(event);
    }
}

void SceneManager::exec()
{
    // We start the clock
    const sf::Clock clock;
    const sf::Clock spawnClock;
    float startSpawn = spawnClock.getElapsedTime().asMilliseconds();
    float previous = clock.getElapsedTime().asMilliseconds();
    auto lag = 0.0;

    int counter = 0;
    while (m_window->isOpen())
    {
        if (const float lastSpawnTick = spawnClock.getElapsedTime().asMilliseconds(); lastSpawnTick - startSpawn > 1000)
        {
            std::cout << "FPS: " << counter << std::endl;
            startSpawn = lastSpawnTick;
            counter = 0;
        }

        const float current = clock.getElapsedTime().asMilliseconds();
        const auto elapsed = current - previous;
        previous = current;
        lag += elapsed;
        m_window->clear();

        processInput();

        while (m_currentScene->getRefreshTime().asMilliseconds() > 0.0
            && lag >= m_currentScene->getRefreshTime().asMilliseconds())
        {
            m_currentScene->update(m_currentScene->getRefreshTime().asSeconds());
            lag -= m_currentScene->getRefreshTime().asMilliseconds();
            ++counter;
        }

        m_currentScene->render();
        m_window->display();
    }
}