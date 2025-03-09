#include "Hero.h"

void Hero::processInput(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    m_isMovingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    m_isMovingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_isMovingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    m_isMovingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}