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
    , m_currentScene(nullptr)
{
    //m_scenes.push_back(std::make_unique<Menu>(m_window.get(), this, 30.f));
    m_scenes.push_back(std::make_unique<Game>(m_window.get(), 60.f));
    //m_scenes.push_back(std::make_unique<Pause>(m_window.get(), 30.f));
    m_currentScene = m_scenes.front().get();
}

void SceneManager::push_back(std::unique_ptr<SceneBase> scene)
{
    m_scenes.push_back(std::move(scene));
}

sf::RenderWindow* SceneManager::getWindow()
{
    return m_window.get();
}

SceneBase* SceneManager::getCurrentScene()
{
    return m_currentScene;
}

void SceneManager::setCurrentScene(const int& index)
{
    m_currentScene = m_scenes[index].get();
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
                if (m_currentScene != m_scenes.front().get())
                {
                    if (m_currentScene == m_scenes[GAME].get())
                        m_currentScene = m_scenes[PAUSE].get();
                    else if (m_currentScene == m_scenes[PAUSE].get())
                        m_currentScene = m_scenes[GAME].get();
                    else if (m_currentScene == m_scenes[CREDIT].get())
                        m_currentScene = m_scenes[MENU].get();
                }
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
            m_currentScene->update(elapsed);
            lag -= m_currentScene->getRefreshTime().asMilliseconds();
            ++counter;
        }

        m_currentScene->render();
        m_window->display();
    }
}