#include "EnemyState.h"

#include <iostream>

#include "Animation.h"
#include "Component.h"
#include "Enemy.h"

namespace MeleeEnemyStates
{
    void PatrolState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " start patrol" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void PatrolState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped patrol" << std::endl;
    }

    void ChaseState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " chasing the player" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void ChaseState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped chasing" << std::endl;
    }

    void AttackState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " attacking the player" << std::endl;
        m_attackTimer = 0.0f;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("attack_down");
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

    void AttackState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped attacking" << std::endl;
    }

    void RetreatState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " retreat !" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void RetreatState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stop retreat" << std::endl;
    }
}

namespace RangedEnemyStates
{
    void PatrolState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " start patrol" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void PatrolState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped patrol" << std::endl;
    }

    void PositioningState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " positioning at optimal distance" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void PositioningState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " found player position" << std::endl;
    }

    void AttackState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " shooting the player" << std::endl;
        m_attackTimer = 0.0f;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("attack_down");
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

    void AttackState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stopped shooting" << std::endl;
    }

    void RetreatState::enterState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " retreat !" << std::endl;

        auto animComp = static_cast<AnimationComponent*>(enemy->getComponent("AnimationComponent"));
        if (animComp)
            animComp->playAnimation("run_down");
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

    void RetreatState::exitState(IEnemy* enemy)
    {
        std::cout << enemy->getName() << " stop retreat" << std::endl;
    }
}