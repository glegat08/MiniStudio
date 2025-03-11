#include "HeroState.h"

#include <memory>

#include "Hero.h"

HeroState::HeroState() : m_currentState(nullptr), m_currentStateName(StateEnum::idle)
{
    m_states[StateEnum::idle] = std::make_shared<Idle>();
    m_states[StateEnum::move] = std::make_shared<Movement>();
    m_states[StateEnum::attack] = std::make_shared<Attack>();
    m_states[StateEnum::dash] = std::make_shared<Dash>();
    m_states[StateEnum::hurt] = std::make_shared<Hurt>();
    m_states[StateEnum::death] = std::make_shared<Death>();

    m_currentState = m_states[StateEnum::idle];
}

HeroState::~HeroState()
{
    m_states.clear();
    m_currentState = nullptr;

    while (!m_stateStack.empty())
    {
        m_stateStack.pop();
    }
}

void HeroState::setState(Hero* hero, HeroStateNames::stateName newState)
{
    if (newState == StateEnum::death)
    {
        while (!m_stateStack.empty())
        {
            m_stateStack.pop();
        }
    }

    if (m_states.find(newState) == m_states.end())
        return;

    m_currentState = m_states[newState];
    m_currentStateName = newState;

    if (hero != nullptr)
        m_currentState->setTexture(*hero);
}