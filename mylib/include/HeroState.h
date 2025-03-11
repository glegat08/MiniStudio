#pragma once
#include <map>
#include <stack>

#include "StateManager.h"

class Hero;

namespace HeroStateNames
{
	enum class stateName;
}

class HeroState
{
public:
	HeroState();
	~HeroState();

	using StateEnum = HeroStateNames::stateName;

	void setState(Hero* hero, HeroStateNames::stateName newState);
	void pushState(Hero* hero, HeroStateNames::stateName newState);
	void popState(Hero* hero);

private:
	IState m_currentState;
};