#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class SceneBase
{
public:
    SceneBase(sf::RenderWindow* window, const float& framerate, const std::string& name = "Scene");
    virtual ~SceneBase() = default;

    virtual void processInput(const sf::Event& event);
    virtual void update(const float& deltaTime);
    virtual void render();

    virtual void addChild(std::unique_ptr<SceneBase> child);
    virtual void removeChild(const std::string& name);
    virtual SceneBase* getChild(const std::string& name);

    sf::Time getRefreshTime() const;
    std::string getName() const;
    void setName(const std::string& name);
    static SceneBase* getCurrentScene();

protected:
    sf::RenderWindow* m_renderWindow;
    sf::Time m_refreshTime;
    std::string m_name;
    std::vector<std::unique_ptr<SceneBase>> m_children;

private:
    static SceneBase* m_currentScene;
};