#include <SFML/Graphics.hpp>

#include "StateManager.h"
#include "hero.h"

bool IState::m_dashAvailable = true;
sf::Clock IState::m_dashCooldownClock;
const float IState::m_dashCooldownDuration = 2.0f;
bool IState::m_mouseRightPressed = false;
bool IState::m_mouseLeftPressed = false;

// ISTATE
bool IState::isGoingUp()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

bool IState::isGoingDown()
{
    return sf::Keyboard::isKeyPressed((sf::Keyboard::S));
}

bool IState::isGoingLeft()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

bool IState::isGoingRight()
{
    return sf::Keyboard::isKeyPressed((sf::Keyboard::D));
}

bool IState::isDashing()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canDash();
}

bool IState::canDash()
{
    if (!m_dashAvailable && m_dashCooldownClock.getElapsedTime().asSeconds() >= m_dashCooldownDuration)
        m_dashAvailable = true;

    return m_dashAvailable;
}

bool IState::isMeleAttacking()
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool result = currentPressed && !m_mouseLeftPressed;
    m_mouseLeftPressed = currentPressed;
    return result;
}

bool IState::isRangeAttacking()
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    bool result = currentPressed && !m_mouseRightPressed;
    m_mouseRightPressed = currentPressed;
    return result;
}

void IState::updateDirection(Hero& hero)
{
	
}

bool IState::isTemporaryState() const
{
    return false;
}


// IDLE STATE
void Idle::handleInput(Hero& hero)
{
    
}

void Idle::update(Hero& hero, float deltaTime)
{
   
}

void Idle::setTexture(Hero& hero)
{
	
}

// ATTACK STATE
void Attack::handleInput(Hero& hero)
{
    updateDirection(hero);
}

void Attack::update(Hero& hero, float deltaTime)
{
    
}

void Attack::setTexture(Hero& hero)
{

}

// MOVING STATE
void Movement::handleInput(Hero& hero)
{
    
}

void Movement::update(Hero& hero, float deltaTime)
{
    
}

void Movement::setTexture(Hero& hero)
{

}

// DASH STATE
void Dash::handleInput(Hero& hero)
{
    
}

void Dash::update(Hero& hero, float deltaTime)
{
    
}

void Dash::setTexture(Hero& hero)
{

}

// HURT STATE
void Hurt::handleInput(Hero& hero)
{
    
}

void Hurt::update(Hero& hero, float deltaTime)
{
    
}

void Hurt::setTexture(Hero& hero)
{
    
}

// DEATH STATE
void Death::handleInput(Hero& hero)
{
}

void Death::update(Hero& hero, float deltaTime)
{
    
}

void Death::setTexture(Hero& hero)
{
    
}