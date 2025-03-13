#include "Game.h"

Game::Game(sf::RenderWindow* window, const float& framerate)
	: SceneBase(window, framerate)
{
	setPlayer();
}

void Game::setPlayer()
{
	m_player.getSprite().setPosition(500, 500);

	m_player.setState(HeroStateNames::stateName::idle);
}

void Game::processInput(const sf::Event& event)
{
	m_player.handleInput();
}

void Game::update(const float& deltaTime)
{
	m_player.update(deltaTime);
}

void Game::render()
{
	m_renderWindow->draw(m_player.getSprite());
}
