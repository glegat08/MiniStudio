#include "EnemyState.h"

#include <iostream>

#include "Component.h"
#include "Enemy.h"

namespace MeleeEnemyStates
{
    void PatrolState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " start patrol" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(200, 50, 50));
    }

    void PatrolState::update(IEnemy* enemy, float deltaTime)
    {
        enemy->patrol(deltaTime);

        if (enemy->canDetectPlayer())
        {
            enemy->changeState(new ChaseState());
            return;
        }
    }

    void PatrolState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped patrol" << std::endl;
    }

    void ChaseState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " chasing the player" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(220, 80, 80));
    }

    void ChaseState::update(IEnemy* enemy, float deltaTime)
    {
        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (enemy->isWithinAttackRange())
        {
            enemy->changeState(new AttackState());
            return;
        }

        if (enemy->shouldRetreat())
        {
            enemy->changeState(new RetreatState());
            return;
        }

        enemy->moveTowardPlayer(deltaTime);
    }

    void ChaseState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped chasing" << std::endl;
    }

    void AttackState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " attacking the player" << std::endl;
        m_attackTimer = 0.0f;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color::Red);
    }

    void AttackState::update(IEnemy* enemy, float deltaTime)
    {
        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (!enemy->isWithinAttackRange())
        {
            enemy->changeState(new ChaseState());
            return;
        }

        if (enemy->shouldRetreat())
        {
            enemy->changeState(new RetreatState());
            return;
        }

        m_attackTimer += deltaTime;
        if (m_attackTimer >= m_attackCooldown)
        {
            enemy->Attack(deltaTime);
            m_attackTimer = 0.0f;
        }
    }

    void AttackState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped attacking" << std::endl;
    }

    void RetreatState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " retreat !" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(100, 0, 0));
    }

    void RetreatState::update(IEnemy* enemy, float deltaTime)
    {
        enemy->retreatFromPlayer(deltaTime);

        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (!enemy->shouldRetreat())
        {
            enemy->changeState(new ChaseState());
            return;
        }
    }

    void RetreatState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stop retreat" << std::endl;
    }
}

namespace RangedEnemyStates
{
    void PatrolState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " start patrol" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(50, 150, 200));
    }

    void PatrolState::update(IEnemy* enemy, float deltaTime)
    {
        enemy->patrol(deltaTime);

        if (enemy->canDetectPlayer())
        {
            enemy->changeState(new PositioningState());
            return;
        }
    }

    void PatrolState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped patrol" << std::endl;
    }

    void PositioningState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " positioning at optimal distance" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(100, 180, 255));
    }

    void PositioningState::update(IEnemy* enemy, float deltaTime)
    {
        auto rangedEnemy = dynamic_cast<RangedEnemy*>(enemy);
        if (!rangedEnemy)
            return;

        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (enemy->shouldRetreat())
        {
            enemy->changeState(new RetreatState());
            return;
        }

        if (rangedEnemy->isAtOptimalDistance())
        {
            enemy->changeState(new AttackState());
            return;
        }

        rangedEnemy->maintainDistance(deltaTime);
    }

    void PositioningState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " found player position" << std::endl;
    }

    void AttackState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " shooting the player" << std::endl;
        m_attackTimer = 0.0f;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color::Cyan);
    }

    void AttackState::update(IEnemy* enemy, float deltaTime)
    {
        auto rangedEnemy = dynamic_cast<RangedEnemy*>(enemy);
        if (!rangedEnemy)
            return;

        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (!rangedEnemy->isAtOptimalDistance())
        {
            enemy->changeState(new PositioningState());
            return;
        }

        if (enemy->shouldRetreat())
        {
            enemy->changeState(new RetreatState());
            return;
        }

        m_attackTimer += deltaTime;
        if (m_attackTimer >= m_attackCooldown)
        {
            enemy->Attack(deltaTime);
            m_attackTimer = 0.0f;
        }
    }

    void AttackState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped shooting" << std::endl;
    }

    void RetreatState::enter(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " retreat !" << std::endl;

        auto renderer = static_cast<SquareRenderer*>(enemy->getComponent("SquareRenderer"));
        if (renderer)
            renderer->setColor(sf::Color(0, 50, 100));
    }

    void RetreatState::update(IEnemy* enemy, float deltaTime)
    {
        enemy->retreatFromPlayer(deltaTime);

        if (!enemy->canDetectPlayer())
        {
            enemy->changeState(new PatrolState());
            return;
        }

        if (!enemy->shouldRetreat())
        {
            enemy->changeState(new PositioningState());
            return;
        }
    }

    void RetreatState::exit(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stop retreat" << std::endl;
    }
}