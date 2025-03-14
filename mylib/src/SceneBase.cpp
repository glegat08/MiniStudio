#include "SceneBase.h"

#include "SceneBase.h"
#include <algorithm>

SceneBase* SceneBase::m_currentScene = nullptr;

SceneBase::SceneBase(sf::RenderWindow* window, const float& framerate, const std::string& name)
    : m_renderWindow(window)
    , m_refreshTime(sf::seconds(1.f / framerate))
    , m_name(name)
{
    m_currentScene = this;
}

void SceneBase::processInput(const sf::Event& event)
{
    for (auto& child : m_children)
        child->processInput(event);
}

void SceneBase::update(const float& deltaTime)
{
    for (auto& child : m_children)
        child->update(deltaTime);
}

void SceneBase::render()
{
    for (auto& child : m_children)
        child->render();
}

void SceneBase::addChild(std::unique_ptr<SceneBase> child)
{
    m_children.push_back(std::move(child));
}

void SceneBase::removeChild(const std::string& name)
{
    auto it = std::find_if(m_children.begin(), m_children.end(),
        [&name](const std::unique_ptr<SceneBase>& scene)
        {
            return scene->getName() == name;
        });

    if (it != m_children.end())
        m_children.erase(it);
}

SceneBase* SceneBase::getChild(const std::string& name)
{
    for (auto& child : m_children)
    {
        if (child->getName() == name)
            return child.get();

        if (SceneBase* found = child->getChild(name))
            return found;
    }
    return nullptr;
}

sf::Time SceneBase::getRefreshTime() const
{
    return m_refreshTime;
}

std::string SceneBase::getName() const
{
    return m_name;
}

void SceneBase::setName(const std::string& name)
{
    m_name = name;
}

SceneBase* SceneBase::getCurrentScene()
{
    return m_currentScene;
}
