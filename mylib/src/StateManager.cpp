#include "StateManager.h"
#include "Hero.h"
#include "Animation.h"

bool IState::m_mouseLeftPressed = false;

// Base State implementation
IState::IState(Hero* owner)
    : m_owner(owner)
{
}

bool IState::isGoingLeft() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

bool IState::isGoingRight() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}

bool IState::isGoingUp() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
}

bool IState::isGoingDown() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}

bool IState::isAttacking() const
{
    bool currentPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool result = currentPressed && !m_mouseLeftPressed;
    m_mouseLeftPressed = currentPressed;
    return result;
}

bool IState::isShooting() const
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void IState::updateDirection()
{
    if (isGoingLeft())
        m_owner->setDirection(Direction::Left);
    else if (isGoingRight())
        m_owner->setDirection(Direction::Right);
    else if (isGoingUp())
        m_owner->setDirection(Direction::Up);
    else if (isGoingDown())
        m_owner->setDirection(Direction::Down);
}

// Idle State Implementation
IdleState::IdleState(Hero* owner)
    : IState(owner)
{
}

void IdleState::enterState()
{
    playStateAnimation();
    m_owner->setState(stateName::idle);
}

void IdleState::handleInput()
{
    bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

    if (moving) 
    {
        updateDirection();
        m_owner->getStateManager().changeState("Run");
    }

    if (isAttacking()) 
        m_owner->getStateManager().changeState("Attack");

    if (isShooting()) 
        m_owner->getStateManager().changeState("Shoot");
}

void IdleState::update(float deltaTime)
{
}

void IdleState::exitState()
{
}

void IdleState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation idleDownAnim("player", idleFrameCount, 0.4f);
    idleDownAnim.setFrameSize(frameSize);
    idleDownAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, downRow * frameSize.y));
    animation_component->addAnimation("idle_down", idleDownAnim);

    Animation idleUpAnim("player", idleFrameCount, 0.4f);
    idleUpAnim.setFrameSize(frameSize);
    idleUpAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, upRow * frameSize.y));
    animation_component->addAnimation("idle_up", idleUpAnim);

    Animation idleRightAnim("player", idleFrameCount, 0.4f);
    idleRightAnim.setFrameSize(frameSize);
    idleRightAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, rightRow * frameSize.y));
    animation_component->addAnimation("idle_right", idleRightAnim);
}

void IdleState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    Direction direction = m_owner->getDirection();
    bool isFacingLeft = m_owner->isFacingLeft();

    std::string anim_name;
    switch (direction)
    {
    case Direction::Up:
        anim_name = "idle_up";
        break;
    case Direction::Down:
        anim_name = "idle_down";
        break;
    case Direction::Left:
    case Direction::Right:
        anim_name = "idle_right";
        break;
    }

    animation_component->playAnimation(anim_name);

    if ((direction == Direction::Left || direction == Direction::Right) && isFacingLeft)
        animation_component->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

// Run State Implementation
RunState::RunState(Hero* owner)
    : IState(owner)
{
}

void RunState::enterState()
{
    playStateAnimation();
    m_owner->setState(stateName::run);
}

void RunState::handleInput()
{
    bool opposingHorizontal = isGoingLeft() && isGoingRight();
    bool opposingVertical = isGoingUp() && isGoingDown();
    bool noMovement = !isGoingLeft() && !isGoingRight() && !isGoingUp() && !isGoingDown();

    if (noMovement || opposingHorizontal || opposingVertical)
    {
        m_owner->getStateManager().changeState("Idle");
        return;
    }

    if (isAttacking())
    {
        m_owner->getStateManager().changeState("Attack");
        return;
    }

    if (isShooting())
    {
        m_owner->getStateManager().changeState("Shoot");
        return;
    }
}

void RunState::update(float deltaTime)
{
    if (m_owner->getCurrentState() != stateName::run)
        return;

    if (isGoingUp())
    {
        m_owner->setDirection(Direction::Up);
        m_owner->setState(stateName::run);
    }
    else if (isGoingDown())
    {
        m_owner->setDirection(Direction::Down);
        m_owner->setState(stateName::run);
    }
    else if (isGoingLeft())
    {
        m_owner->setDirection(Direction::Left);
        m_owner->setFacingLeft(true);
        m_owner->setState(stateName::run);
    }
    else if (isGoingRight())
    {
        m_owner->setDirection(Direction::Right);
        m_owner->setFacingLeft(false);
        m_owner->setState(stateName::run);
    }
}

void RunState::exitState()
{
}

void RunState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation runDownAnim("player", runFrameCount, 0.1f);
    runDownAnim.setFrameSize(frameSize);
    runDownAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, downRow * frameSize.y));
    animation_component->addAnimation("run_down", runDownAnim);

    Animation runUpAnim("player", runFrameCount, 0.1f);
    runUpAnim.setFrameSize(frameSize);
    runUpAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, upRow * frameSize.y));
    animation_component->addAnimation("run_up", runUpAnim);

    Animation runRightAnim("player", runFrameCount, 0.1f);
    runRightAnim.setFrameSize(frameSize);
    runRightAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, rightRow * frameSize.y));
    animation_component->addAnimation("run_right", runRightAnim);
}

void RunState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    Direction direction = m_owner->getDirection();
    bool isFacingLeft = m_owner->isFacingLeft();

    std::string anim_name;
    switch (direction)
    {
    case Direction::Up:
        anim_name = "idle_up";
        break;
    case Direction::Down:
        anim_name = "idle_down";
        break;
    case Direction::Left:
    case Direction::Right:
        anim_name = "idle_right";
        break;
    }

    animation_component->playAnimation(anim_name);

    if ((direction == Direction::Left || direction == Direction::Right) && isFacingLeft)
        animation_component->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

// Attack State Implementation
AttackState::AttackState(Hero* owner)
    : IState(owner)
    , m_initialVelocity(0.0f, 0.0f)
    , m_decelerationRate(5.0f)
{
}

void AttackState::enterState()
{
    auto player_controller = static_cast<PlayerController*>(m_owner->getComponent("PlayerController"));
    if (player_controller)
    {
        m_initialVelocity = sf::Vector2f(0.0f, 0.0f);
        float speed = m_owner->getSpeed() * 0.7f;

        if (isGoingUp())
            m_initialVelocity.y = -speed;
        if (isGoingDown())
            m_initialVelocity.y = speed;
        if (isGoingLeft())
            m_initialVelocity.x = -speed;
        if (isGoingRight())
            m_initialVelocity.x = speed;

        float length = std::sqrt(m_initialVelocity.x * m_initialVelocity.x +
            m_initialVelocity.y * m_initialVelocity.y);

        if (length > 0.0f)
        {
            m_initialVelocity.x = m_initialVelocity.x / length * speed;
            m_initialVelocity.y = m_initialVelocity.y / length * speed;
        }

        disableMovement();
    }

    playStateAnimation();
    m_owner->setState(stateName::attack);
    m_animClock.restart();
}

void AttackState::handleInput()
{
    disableMovement();
}

void AttackState::update(float deltaTime)
{
    float slowdown_factor = std::exp(-m_decelerationRate * deltaTime);
    m_initialVelocity *= slowdown_factor;

    if (std::abs(m_initialVelocity.x) > 0.5f || std::abs(m_initialVelocity.y) > 0.5f)
        m_owner->move(m_initialVelocity * deltaTime);

    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (animation_component && animation_component->isAnimationFinished())
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void AttackState::exitState()
{
}

void AttackState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation attackDownAnim("player", attackFrameCount, 0.1f, false);
    attackDownAnim.setFrameSize(frameSize);
    attackDownAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, downRow * frameSize.y));
    animation_component->addAnimation("attack_down", attackDownAnim);

    Animation attackUpAnim("player", attackFrameCount, 0.1f, false);
    attackUpAnim.setFrameSize(frameSize);
    attackUpAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, upRow * frameSize.y));
    animation_component->addAnimation("attack_up", attackUpAnim);

    Animation attackRightAnim("player", attackFrameCount, 0.1f, false);
    attackRightAnim.setFrameSize(frameSize);
    attackRightAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, rightRow * frameSize.y));
    animation_component->addAnimation("attack_right", attackRightAnim);
}

void AttackState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    Direction direction = m_owner->getDirection();
    bool isFacingLeft = m_owner->isFacingLeft();

    std::string anim_name;
    switch (direction)
    {
    case Direction::Up:
        anim_name = "idle_up";
        break;
    case Direction::Down:
        anim_name = "idle_down";
        break;
    case Direction::Left:
    case Direction::Right:
        anim_name = "idle_right";
        break;
    }

    animation_component->playAnimation(anim_name);

    if ((direction == Direction::Left || direction == Direction::Right) && isFacingLeft)
        animation_component->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

void AttackState::disableMovement()
{
    auto player_controller = static_cast<PlayerController*>(m_owner->getComponent("PlayerController"));
    if (player_controller)
    {
        player_controller->m_isMovingUp = false;
        player_controller->m_isMovingDown = false;
        player_controller->m_isMovingLeft = false;
        player_controller->m_isMovingRight = false;
    }
}

// Shoot State Implementation
ShootState::ShootState(Hero* owner)
    : IState(owner)
{
}

void ShootState::enterState()
{
    playStateAnimation();
    m_owner->setState(stateName::shoot);
    m_animClock.restart();
}

void ShootState::handleInput()
{
}

void ShootState::update(float deltaTime)
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (animation_component && animation_component->isAnimationFinished()) 
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void ShootState::exitState()
{
}

void ShootState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation shootDownAnim("player", shootFrameCount, 0.1f, false);
    shootDownAnim.setFrameSize(frameSize);
    shootDownAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, downRow * frameSize.y));
    animation_component->addAnimation("shoot_down", shootDownAnim);

    Animation shootUpAnim("player", shootFrameCount, 0.1f, false);
    shootUpAnim.setFrameSize(frameSize);
    shootUpAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, upRow * frameSize.y));
    animation_component->addAnimation("shoot_up", shootUpAnim);

    Animation shootRightAnim("player", shootFrameCount, 0.1f, false);
    shootRightAnim.setFrameSize(frameSize);
    shootRightAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, rightRow * frameSize.y));
    animation_component->addAnimation("shoot_right", shootRightAnim);

}

void ShootState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    Direction direction = m_owner->getDirection();
    bool isFacingLeft = m_owner->isFacingLeft();

    std::string anim_name;
    switch (direction)
    {
    case Direction::Up:
        anim_name = "idle_up";
        break;
    case Direction::Down:
        anim_name = "idle_down";
        break;
    case Direction::Left:
    case Direction::Right:
        anim_name = "idle_right";
        break;
    }

    animation_component->playAnimation(anim_name);

    if ((direction == Direction::Left || direction == Direction::Right) && isFacingLeft)
        animation_component->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

// Hurt State Implementation
HurtState::HurtState(Hero* owner)
    : IState(owner)
{
}

void HurtState::enterState()
{
    playStateAnimation();
    m_owner->setState(stateName::hurt);
    m_animClock.restart();
}

void HurtState::handleInput()
{
}

void HurtState::update(float deltaTime)
{
    if (m_animClock.getElapsedTime().asSeconds() >= m_hurtDuration ||
        static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"))->isAnimationFinished()) 
    {
        bool moving = isGoingLeft() || isGoingRight() || isGoingUp() || isGoingDown();

        if (moving)
            m_owner->getStateManager().changeState("Run");
        else
            m_owner->getStateManager().changeState("Idle");
    }
}

void HurtState::exitState()
{
}

void HurtState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation hurtAnim("player", hurtFrameCount, 1.f, false);
    hurtAnim.setFrameSize(frameSize);
    hurtAnim.setStartPosition(sf::Vector2i(0, hurtRow * frameSize.y));
    animation_component->addAnimation("hurt", hurtAnim);
}

void HurtState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    animation_component->playAnimation("hurt");

    animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}

// Death State Implementation
DeathState::DeathState(Hero* owner)
    : IState(owner)
{
}

void DeathState::enterState()
{
    playStateAnimation();
    m_owner->setState(stateName::death);
}

void DeathState::handleInput()
{
}

void DeathState::update(float deltaTime)
{
}

void DeathState::exitState()
{
}

void DeathState::configureAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    const sf::Vector2i frameSize(32, 32);

    Animation deathAnim("player", deathFrameCount, 0.2f, false);
    deathAnim.setFrameSize(frameSize);
    deathAnim.setStartPosition(sf::Vector2i(0, deathRow * frameSize.y));
    animation_component->addAnimation("death", deathAnim);
}

void DeathState::playStateAnimation()
{
    auto animation_component = static_cast<AnimationComponent*>(m_owner->getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    animation_component->playAnimation("death");

    animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
}
