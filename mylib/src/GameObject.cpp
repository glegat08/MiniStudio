#include "GameObject.h"

void IGameObject::processInput(const sf::Event& event)
{
    for (auto& gameObject : m_allGameObjects)
        gameObject->processInput(event);
}
