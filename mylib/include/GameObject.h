#pragma once
#include "Game.h"

class IGameObject
{
public:
	virtual ~IGameObject() = default;

	virtual int getHp() = 0;
	virtual void getStat() = 0;
	virtual void processInput(const sf::Event& event) = 0;

private:
	std::vector<IGameObject*> m_allGameObjects;

};