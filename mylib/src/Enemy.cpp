#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <random>

#include "Animation.h"
#include "Collision.h"
#include "Game.h"
#include "Hero.h"

// MeleeEnemy
MeleeEnemy::MeleeEnemy(const std::string& name)
    : IEnemy(name)
    , m_attackCD(1.0f)
    , m_currentAttackCD(0.0f)
    , m_isAttacking(false)
{
    addTag("MeleeEnemy");
    m_damage = 15;
    m_attackRadius = 60.0f;
    m_speed = 100.0f;
}

MeleeEnemy::~MeleeEnemy()
{
}

void MeleeEnemy::init(const sf::Vector2f& pos)
{
    IEnemy::init(pos, 40.0f, sf::Color::Transparent, m_speed);

    configureAnimation();

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (animation_component)
        animation_component->playAnimation("run_down");

    auto enemyHitbox = std::make_shared<Hitbox>(sf::Vector2f(60.f, 60.f));
    enemyHitbox->setDebugDraw(false);
    addComponent(enemyHitbox);

    changeState(new MeleeEnemyStates::PatrolState());
}

void MeleeEnemy::Attack(float deltaTime)
{
    if (m_currentAttackCD > 0)
    {
        m_currentAttackCD -= deltaTime;
        return;
    }

    if (isWithinAttackRange())
    {
        std::cout << getName() << " performs a melee attack!" << std::endl;
        m_isAttacking = true;

        m_currentAttackCD = m_attackCD;
    }

    m_isAttacking = false;
}

void MeleeEnemy::Block(float deltaTime)
{
    std::cout << getName() << " is blocking!" << std::endl;
}

bool MeleeEnemy::canCounterAttack() const
{
    return false;
}

void MeleeEnemy::configureAnimation()
{
    downAnimation();
    sideAnimation();
    upAnimation();
    hurtSprite();
    deathAnimation();

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (animation_component)
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

void MeleeEnemy::downAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runDownAnim("enemy", runFrameCount, 0.15f);
    runDownAnim.setFrameSize(frameSize);
    runDownAnim.setStartPosition(sf::Vector2i(runDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_down", runDownAnim);

    Animation attackDownAnim("enemy", attackFrameCount, 0.1f, true);
    attackDownAnim.setFrameSize(frameSize);
    attackDownAnim.setStartPosition(sf::Vector2i(attackDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_down", attackDownAnim);
}

void MeleeEnemy::sideAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runRightAnim("enemy", runFrameCount, 0.15f);
    runRightAnim.setFrameSize(frameSize);
    runRightAnim.setStartPosition(sf::Vector2i(runRightOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_right", runRightAnim);

    Animation attackRightAnim("enemy", attackFrameCount, 0.1f, true);
    attackRightAnim.setFrameSize(frameSize);
    attackRightAnim.setStartPosition(sf::Vector2i(attackRightOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_right", attackRightAnim);
}

void MeleeEnemy::upAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runUpAnim("enemy", runFrameCount, 0.15f);
    runUpAnim.setFrameSize(frameSize);
    runUpAnim.setStartPosition(sf::Vector2i(runUpOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_up", runUpAnim);

    Animation attackUpAnim("enemy", attackFrameCount, 0.1f, true);
    attackUpAnim.setFrameSize(frameSize);
    attackUpAnim.setStartPosition(sf::Vector2i(attackUpOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_up", attackUpAnim);
}

void MeleeEnemy::hurtSprite()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation hurtAnim("enemy", hurtFrameCount, 0.3f, false);
    hurtAnim.setFrameSize(frameSize);
    hurtAnim.setStartPosition(sf::Vector2i(hurtDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("hurt", hurtAnim);
}

void MeleeEnemy::deathAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation deathAnim("enemy", deathFrameCount, 0.2f, false);
    deathAnim.setFrameSize(frameSize);
    deathAnim.setStartPosition(sf::Vector2i(deathOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("death", deathAnim);
}

// RangedEnemy
RangedEnemy::RangedEnemy(const std::string& name)
    : IEnemy(name)
    , m_optimalDistance(200.0f)
    , m_fireCD(2.0f)
    , m_currentFireCD(0.0f)
{
    addTag("RangedEnemy");
    m_damage = 10;
    m_attackRadius = 300.0f;
    m_speed = 100.0f;
}

RangedEnemy::~RangedEnemy()
{
}

void RangedEnemy::init(const sf::Vector2f& pos)
{
    IEnemy::init(pos, 35.0f, sf::Color::Transparent, m_speed);

    configureAnimation();

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (animation_component)
        animation_component->playAnimation("run_down");

    auto rangedEnemyHitbox = std::make_shared<Hitbox>(sf::Vector2f(60.f, 60.f));
    rangedEnemyHitbox->setDebugDraw(false);
    addComponent(rangedEnemyHitbox);

    changeState(new RangedEnemyStates::PatrolState());
}

void RangedEnemy::Attack(float deltaTime)
{
    if (isWithinAttackRange() && isAtOptimalDistance())
    {
        std::cout << getName() << " shooting the player!" << std::endl;

        auto enemy_render = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (enemy_render)
        {
            sf::Vector2f enemyPos = enemy_render->getPosition();

            sf::Vector2f direction = m_playerPos - enemyPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length > 0)
            {
                direction /= length;

                auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
                if (animation_component)
                {
                    std::string animationName;
                    switch (m_currentDirection)
                    {
                    case EnemyDirection::Up:
                        animationName = "attack_up";
                        break;
                    case EnemyDirection::Down:
                        animationName = "attack_down";
                        break;
                    case EnemyDirection::Left:
                    case EnemyDirection::Right:
                        animationName = "attack_right";
                        break;
                    }
                    animation_component->playAnimation(animationName);
                }
                if (Game::getInstance())
                {
                    sf::Vector2f arrowPos = enemyPos + direction * 30.f;
                    Game::getInstance()->createArrow(arrowPos, direction, m_damage);
                }
            }
        }
    }
}

void RangedEnemy::Block(float deltaTime)
{
    std::cout << getName() << " is blocking!" << std::endl;
}

bool RangedEnemy::isAtOptimalDistance() const
{
    if (!canDetectPlayer())
        return false;

    auto square_renderer = static_cast<const SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return false;

    sf::Vector2f pos = square_renderer->getPosition();
    float distanceSq = (pos.x - m_playerPos.x) * (pos.x - m_playerPos.x) +
        (pos.y - m_playerPos.y) * (pos.y - m_playerPos.y);

    float distance = std::sqrt(distanceSq);
    float tolerance = 0.2f * m_optimalDistance;

    return std::abs(distance - m_optimalDistance) <= tolerance;
}

void RangedEnemy::maintainDistance(float deltaTime)
{
    if (!canDetectPlayer())
        return;

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f currentPos = square_renderer->getPosition();
    sf::Vector2f direction = m_playerPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance == 0)
        return;

    direction /= distance;

    if (distance < m_optimalDistance * 0.8f)
        square_renderer->setPosition(currentPos - direction * m_speed * deltaTime);
    else if (distance > m_optimalDistance * 1.2f)
        square_renderer->setPosition(currentPos + direction * m_speed * deltaTime);
}

void RangedEnemy::configureAnimation()
{
    downAnimation();
    sideAnimation();
    upAnimation();
    hurtSprite();
    deathAnimation();

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (animation_component)
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

void RangedEnemy::downAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runDownAnim("enemy", runFrameCount, 0.15f);
    runDownAnim.setFrameSize(frameSize);
    runDownAnim.setStartPosition(sf::Vector2i(runDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_down", runDownAnim);

    Animation attackDownAnim("enemy", attackFrameCount, 0.1f, true);
    attackDownAnim.setFrameSize(frameSize);
    attackDownAnim.setStartPosition(sf::Vector2i(attackDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_down", attackDownAnim);
}

void RangedEnemy::sideAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runRightAnim("enemy", runFrameCount, 0.15f);
    runRightAnim.setFrameSize(frameSize);
    runRightAnim.setStartPosition(sf::Vector2i(runRightOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_right", runRightAnim);

    Animation attackRightAnim("enemy", attackFrameCount, 0.1f, true);
    attackRightAnim.setFrameSize(frameSize);
    attackRightAnim.setStartPosition(sf::Vector2i(attackRightOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_right", attackRightAnim);
}

void RangedEnemy::upAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runUpAnim("enemy", runFrameCount, 0.15f);
    runUpAnim.setFrameSize(frameSize);
    runUpAnim.setStartPosition(sf::Vector2i(runUpOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("run_up", runUpAnim);

    Animation attackUpAnim("enemy", attackFrameCount, 0.1f, true);
    attackUpAnim.setFrameSize(frameSize);
    attackUpAnim.setStartPosition(sf::Vector2i(attackUpOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("attack_up", attackUpAnim);
}

void RangedEnemy::hurtSprite()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation hurtAnim("enemy", hurtFrameCount, 0.3f, false);
    hurtAnim.setFrameSize(frameSize);
    hurtAnim.setStartPosition(sf::Vector2i(hurtDownOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("hurt", hurtAnim);
}

void RangedEnemy::deathAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation deathAnim("enemy", deathFrameCount, 0.2f, false);
    deathAnim.setFrameSize(frameSize);
    deathAnim.setStartPosition(sf::Vector2i(deathOffset * frameSize.x, rowIndex * frameSize.y));
    animation_component->addAnimation("death", deathAnim);
}
