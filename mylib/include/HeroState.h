#pragma once
#include <memory>
#include <map>
#include <string>
#include "StateManager.h"

enum class stateName;
class Hero;
class IState;

enum class Direction;
enum class stateName;

class HeroState
{
public:
    HeroState(Hero* owner);
    ~HeroState();

    void initialize();
    void update(float deltaTime);
    void handleInput();

    void changeState(const std::string& stateName);
    std::string getCurrentStateName() const;

private:
    std::map<std::string, std::unique_ptr<IState>> m_states;
    IState* m_currentState;
    std::string m_currentStateName;
    Hero* m_owner;
};