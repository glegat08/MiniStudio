#pragma once
#include "StateManager.h"


class HeroState
{
public:
	~HeroState();


private:
	IState m_currentState;
};