#include "HeroState.h"
#include "Hero.h"
#include "StateManager.h"

HeroState::HeroState(Hero* owner)
    : m_currentState(nullptr)
    , m_currentStateName("")
    , m_owner(owner)
{
}

HeroState::~HeroState()
{
    m_states.clear();
    m_currentState = nullptr;
}

void HeroState::initialize()
{
    m_states["Idle"] = std::make_unique<IdleState>(m_owner);
    m_states["Run"] = std::make_unique<RunState>(m_owner);
    m_states["Attack"] = std::make_unique<AttackState>(m_owner);
    m_states["Shoot"] = std::make_unique<ShootState>(m_owner);
    m_states["Hurt"] = std::make_unique<HurtState>(m_owner);
    m_states["Death"] = std::make_unique<DeathState>(m_owner);

    for (auto& state : m_states)
    {
        state.second->configureAnimation();
    }

    changeState("Idle");
}

void HeroState::update(float deltaTime)
{
    if (m_currentState)
        m_currentState->update(deltaTime);
}

void HeroState::handleInput()
{
    if (m_currentState)
        m_currentState->handleInput();
}

void HeroState::changeState(const std::string& stateName)
{
    auto iterator = m_states.find(stateName);
    if (iterator == m_states.end())
        return;

    if (m_currentState)
        m_currentState->exitState();

    m_currentState = iterator->second.get();
    m_currentStateName = stateName;

    m_currentState->enterState();
}

std::string HeroState::getCurrentStateName() const
{
    return m_currentStateName;
}