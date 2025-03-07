#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class SceneBase
{
public:
    SceneBase(sf::RenderWindow* window, const float& framerate);
    virtual ~SceneBase() = default;

    virtual void processInput(const sf::Event& event) = 0;
    virtual void update(const float& deltaTime) = 0;
    virtual void render() = 0;

    sf::Time getRefreshTime() const;

protected:
    sf::RenderWindow* m_renderWindow;
    sf::Time m_refreshTime;
};