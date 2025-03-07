#include "SceneBase.h"

SceneBase::SceneBase(sf::RenderWindow* window, const float& framerate)
    : m_renderWindow(window)
    , m_refreshTime(sf::seconds(1.f / framerate))
{

}

sf::Time SceneBase::getRefreshTime() const
{
    return m_refreshTime;
}