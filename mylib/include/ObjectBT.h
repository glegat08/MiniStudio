//#pragma once
//
//#include "behaviorTree.h"
//#include "enemy.h"
//#include "game.h"
//
//class Game;
//
//namespace BT
//{
//    // Base Conditional Nodes
//    template<typename EnemyType>
//    class DetectPlayer : public BehaviorNodeDecorator<EnemyType, IConditionalNode>
//    {
//    public:
//        DetectPlayer(ICompositeNode* node) : BehaviorNodeDecorator<EnemyType, IConditionalNode>(node) {}
//        bool condition() override
//        {
//            return this->getGameObject()->canDetectPlayer();
//        }
//    };
//
//    template<typename EnemyType>
//    class WithinAttackRange : public BehaviorNodeDecorator<EnemyType, IConditionalNode>
//    {
//    public:
//        WithinAttackRange(ICompositeNode* node) : BehaviorNodeDecorator<EnemyType, IConditionalNode>(node) {}
//        bool condition() override
//        {
//            return this->getGameObject()->isWithinAttackRange();
//        }
//    };
//
//    template<typename EnemyType>
//    class ShouldRetreat : public BehaviorNodeDecorator<EnemyType, IConditionalNode>
//    {
//    public:
//        ShouldRetreat(ICompositeNode* node) : BehaviorNodeDecorator<EnemyType, IConditionalNode>(node) {}
//        bool condition() override
//        {
//            return this->getGameObject()->shouldRetreat();
//        }
//    };
//
//    template<typename EnemyType>
//    class HealthLow : public BehaviorNodeDecorator<EnemyType, IConditionalNode>
//    {
//    public:
//        HealthLow(ICompositeNode* node) : BehaviorNodeDecorator<EnemyType, IConditionalNode>(node) {}
//        bool condition() override
//        {
//            return this->getGameObject()->getHp() < this->getGameObject()->getLowHealthThreshold();
//        }
//    };
//
//    template<typename EnemyType>
//    class CheckTargetInRange : public BehaviorNodeDecorator<EnemyType, IConditionalNode>
//    {
//    private:
//        float m_range;
//    public:
//        CheckTargetInRange(ICompositeNode* parent, float range)
//            : BehaviorNodeDecorator<EnemyType, IConditionalNode>(parent), m_range(range) {}
//
//        bool condition() override
//        {
//            auto enemy = this->getGameObject();
//            if (!enemy || !enemy->getCurrentTarget())
//                return false;
//
//            sf::Vector2f enemyPos = enemy->getPosition();
//            sf::Vector2f targetPos = enemy->getCurrentTarget()->getPlayerPosition();
//            float dx = enemyPos.x - targetPos.x;
//            float dy = enemyPos.y - targetPos.y;
//            float dist = std::sqrt(dx * dx + dy * dy);
//            return dist <= m_range;
//        }
//    };
//
//    // Base Action Nodes
//    template<typename EnemyType>
//    class PatrolAction : public BehaviorNodeDecorator<EnemyType, IActionNode>
//    {
//    public:
//        PatrolAction(ICompositeNode* parent)
//            : BehaviorNodeDecorator<EnemyType, IActionNode>(parent) {}
//
//        Status tick() override
//        {
//            float deltaTime = 0.016f;
//            this->getGameObject()->patrol(deltaTime);
//            return Running;
//        }
//    };
//
//    template<typename EnemyType>
//    class ChaseAction : public BehaviorNodeDecorator<EnemyType, IActionNode>
//    {
//    private:
//        Game* m_game;
//    public:
//        ChaseAction(ICompositeNode* parent, Game* game)
//            : BehaviorNodeDecorator<EnemyType, IActionNode>(parent), m_game(game) {}
//
//        Status tick() override
//        {
//            if (m_game && !m_game->getPlayer().isAlive())
//                return Failed;
//
//            float deltaTime = 0.016f;
//            this->getGameObject()->moveTowardPlayer(deltaTime);
//            return Running;
//        }
//    };
//
//    template<typename EnemyType>
//    class AttackAction : public BehaviorNodeDecorator<EnemyType, IActionNode>
//    {
//    private:
//        Game* m_game;
//        int m_attackId;
//    public:
//        AttackAction(ICompositeNode* parent, Game* game, int attackId = 0)
//            : BehaviorNodeDecorator<EnemyType, IActionNode>(parent), m_game(game), m_attackId(attackId) {}
//
//        Status tick() override
//        {
//            if (m_game && !m_game->getPlayer().isAlive())
//                return Failed;
//
//            int damage = this->getGameObject()->performAttack(m_attackId);
//
//            if (m_game)
//                m_game->getPlayer().takeDamage(damage);
//
//            return Success;
//        }
//    };
//
//    template<typename EnemyType>
//    class IdleAction : public BehaviorNodeDecorator<EnemyType, IActionNode>
//    {
//    public:
//        IdleAction(ICompositeNode* parent)
//            : BehaviorNodeDecorator<EnemyType, IActionNode>(parent) {}
//
//        Status tick() override
//        {
//            this->getGameObject()->idle();
//            return Success;
//        }
//    };
//
//    template<typename EnemyType>
//    class RetreatAction : public BehaviorNodeDecorator<EnemyType, IActionNode>
//    {
//    public:
//        RetreatAction(ICompositeNode* parent)
//            : BehaviorNodeDecorator<EnemyType, IActionNode>(parent) {}
//
//        Status tick() override
//        {
//            float deltaTime = 0.016f;
//            this->getGameObject()->retreatFromPlayer(deltaTime);
//            return Running;
//        }
//    };
//
//    class AttackOrChaseSelector : public ICompositeNode
//    {
//    public:
//        AttackOrChaseSelector(ICompositeNode* parent)
//            : ICompositeNode(parent) {
//        }
//
//        Status tick() override
//        {
//            for (auto child : getChildren())
//            {
//                Status status = child->tick();
//                if (status == Success || status == Running)
//                    return status;
//            }
//            return Failed;
//        }
//    };
//
//    class Wait : public IActionNode
//    {
//    private:
//        sf::Time m_duration;
//        sf::Clock m_timer;
//        bool m_started = false;
//
//    public:
//        Wait(ICompositeNode* parent, float duration)
//            : IActionNode(parent), m_duration(sf::seconds(duration)) {
//        }
//
//        Status tick() override
//        {
//            if (!m_started)
//            {
//                m_started = true;
//                m_timer.restart();
//            }
//
//            if (m_timer.getElapsedTime() >= m_duration)
//            {
//                reset();
//                return Success;
//            }
//
//            return Running;
//        }
//
//        void reset()
//        {
//            m_started = false;
//            m_timer.restart();
//        }
//    };
//}