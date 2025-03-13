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
    std::shared_ptr<IState> getCurrentState() const;
    HeroStateNames::stateName getCurrentStateName() const;

    void handleInput(Hero& hero);
    void update(Hero& hero, float deltaTime);

    bool isStateStackEmpty() const { return m_stateStack.empty(); }

private:
    std::map<StateEnum, std::shared_ptr<IState>> m_states;
    std::shared_ptr<IState> m_currentState;
    std::stack<std::pair<StateEnum, std::shared_ptr<IState>>> m_stateStack;
    StateEnum m_currentStateName;
};