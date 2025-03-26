#pragma once
#include <memory>
#include <vector>

#include "sceneBase.h"

class SceneManager
{
public:
    SceneManager(const int& width, const int& height, const std::string& title);

    void addScene(std::unique_ptr<SceneBase> scene);
    sf::RenderWindow* getWindow();
    SceneBase* getCurrentScene();
    void setCurrentScene(const std::string& name);
    SceneBase* getChild(const std::string& name);
    void processInput();
    void exec();

private:
    std::unique_ptr<sf::RenderWindow> m_window;
    std::unique_ptr<SceneBase> m_rootScene;
    SceneBase* m_currentScene;
};