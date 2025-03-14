#pragma once

class IEnemy;

class IEnemyState
{
public:
    virtual ~IEnemyState() = default;

    virtual void enter(IEnemy* enemy) = 0;
    virtual void update(IEnemy* enemy, float deltaTime) = 0;
    virtual void exit(IEnemy* enemy) = 0;
};

namespace MeleeEnemyStates
{
    class PatrolState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };

    class ChaseState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };

    class AttackState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;

    private:
        float m_attackTimer = 0.0f;
        const float m_attackCooldown = 1.0f;
    };

    class RetreatState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };
}

namespace RangedEnemyStates
{
    class PatrolState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };

    class PositioningState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };

    class AttackState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;

    private:
        float m_attackTimer = 0.0f;
        const float m_attackCooldown = 2.0f;
    };

    class RetreatState : public IEnemyState
    {
    public:
        void enter(IEnemy* enemy) override;
        void update(IEnemy* enemy, float deltaTime) override;
        void exit(IEnemy* enemy) override;
    };
}