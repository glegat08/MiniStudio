#include "HeroState.h"
#include "Hero.h"
#include <iostream>

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
    if (m_currentStateName == StateEnum::death && newState != StateEnum::death)
        return;

    if (newState == StateEnum::death)
    {
        while (!m_stateStack.empty())
        {
            m_stateStack.pop();
        }
    }

    if (m_states.find(newState) == m_states.end())
    {
        return;
    }

    m_currentState = m_states[newState];
    m_currentStateName = newState;

    if (hero != nullptr)
        m_currentState->setTexture(*hero);
}

void HeroState::pushState(Hero* hero, HeroStateNames::stateName newState)
{
    if (m_currentStateName == StateEnum::death)
        return;

    if (m_states.find(newState) == m_states.end())
    {
        return;
    }

    m_stateStack.push(std::make_pair(m_currentStateName, m_currentState));

    m_currentState = m_states[newState];
    m_currentStateName = newState;

    if (hero != nullptr)
        m_currentState->setTexture(*hero);
}

void HeroState::popState(Hero* hero)
{
    if (m_stateStack.empty())
    {
        setState(hero, StateEnum::idle);
        return;
    }

    auto previousState = m_stateStack.top();
    m_stateStack.pop();

    m_currentState = previousState.second;
    m_currentStateName = previousState.first;

    if (hero != nullptr)
        m_currentState->setTexture(*hero);
}

std::shared_ptr<IState> HeroState::getCurrentState() const
{
    return m_currentState;
}

HeroStateNames::stateName HeroState::getCurrentStateName() const
{
    return m_currentStateName;
}

void HeroState::handleInput(Hero& hero)
{
    if (m_currentState != nullptr)
        m_currentState->handleInput(hero);
}

void HeroState::update(Hero& hero, float deltaTime)
{
    if (m_currentState != nullptr)
        m_currentState->update(hero, deltaTime);
}