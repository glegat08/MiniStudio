#include "Hero.h"
#include "TextureManager.h"
#include "Component.h"
#include "Animation.h"
#include "Collision.h"

Hero::Hero(const std::string& name)
    : CompositeGameObject(name)
    , m_health(100)
    , m_armor(0)
    , m_strength(10)
    , m_currentStateName(stateName::idle)
    , m_currentDirection(Direction::Down)
    , m_isFacingLeft(false)
    , m_speed(200.0f)
    , m_stateManager(this)
{
    setCategory("Player");
    addTag("Hero");
}

void Hero::initialize(const sf::Vector2f& position, const float& size, const sf::Color& color, const float& speed)
{
    auto renderer = std::make_shared<SquareRenderer>(size, color);
    this->addComponent(renderer);
    renderer->setPosition(position);

    auto controller = std::make_shared<PlayerController>(speed);
    addComponent(controller);

    auto animComp = std::make_shared<AnimationComponent>();
    addComponent(animComp);

    auto hitbox = std::make_shared<Hitbox>(sf::Vector2f(60.f, 60.f));
    hitbox->setDebugDraw(true);
    addComponent(hitbox);

    m_speed = speed;

    m_stateManager.initialize();
}

void Hero::update(const float& deltaTime)
{
    if (m_knockBackDuration > 0)
    {
        m_knockBackDuration -= deltaTime;

        auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (renderer)
        {
            sf::Vector2f currentPos = renderer->getPosition();

            float slowdownFactor = std::min(1.0f, 4.0f * deltaTime);
            m_knockBack *= (1.0f - slowdownFactor);

            renderer->setPosition(currentPos + m_knockBack * deltaTime);
        }
    }

    CompositeGameObject::update(deltaTime);
    m_stateManager.update(deltaTime);
    updateAnimationPosition();
}

void Hero::updateAnimationPosition()
{
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (renderer && animComp) 
    {
        sf::Vector2f position = renderer->getPosition();
        animComp->updatePosition(position);
    }
}

void Hero::processInput(const sf::Event& event)
{
    CompositeGameObject::processInput(event);
    m_stateManager.handleInput();
}

bool Hero::isAlive() const
{
    return m_health > 0;
}

void Hero::takeDamage(int amount)
{
    int actualDamage = amount - m_armor;
    if (actualDamage < 1) 
        actualDamage = 1;

    m_health -= actualDamage;

    if (m_health <= 0) 
    {
        m_health = 0;
        m_stateManager.changeState("Death");
    }
    else
        m_stateManager.changeState("Hurt");
}

void Hero::move(const sf::Vector2f& offset)
{
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (renderer) 
    {
        sf::Vector2f currentPos = renderer->getPosition();
        renderer->setPosition(currentPos + offset);
    }
}

void Hero::setDirection(Direction dir)
{
    m_currentDirection = dir;

    if (dir == Direction::Left) 
        m_isFacingLeft = true;
    else if (dir == Direction::Right) 
        m_isFacingLeft = false;
}

Direction Hero::getDirection() const
{
    return m_currentDirection;
}

void Hero::setFacingLeft(bool left)
{
    m_isFacingLeft = left;

    if (left)
        m_currentDirection = Direction::Left;
    else
        m_currentDirection = Direction::Right;
}

void Hero::attack()
{
    m_stateManager.changeState("Attack");
}

void Hero::shoot()
{
    m_stateManager.changeState("Shoot");
}

stateName Hero::getCurrentState() const
{
    return m_currentStateName;
}

void Hero::setState(stateName newState)
{
    m_currentStateName = newState;

    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animComp)
        return;

    std::string directionSuffix;
    switch (m_currentDirection)
    {
    case Direction::Up:
        directionSuffix = "_up";
        break;
    case Direction::Down:
        directionSuffix = "_down";
        break;
    case Direction::Right:
    case Direction::Left:
        directionSuffix = "_right";
        break;
    }

    if (newState == stateName::hurt)
    {
        animComp->playAnimation("hurt");
        return;
    }

    if (newState == stateName::death)
    {
        animComp->playAnimation("death");
        return;
    }

    switch (newState)
    {
    case stateName::idle:
        animComp->playAnimation("idle" + directionSuffix);
        break;
    case stateName::run:
        animComp->playAnimation("run" + directionSuffix);
        break;
    case stateName::attack:
        animComp->playAnimation("attack" + directionSuffix);
        break;
    case stateName::shoot:
        animComp->playAnimation("shoot" + directionSuffix);
        break;
    default:
        break;
    }

    if (m_isFacingLeft && (m_currentDirection == Direction::Left || m_currentDirection == Direction::Right))
        animComp->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animComp->setScale(sf::Vector2f(2.0f, 2.0f));
}

float Hero::getSpeed() const
{
    return m_speed;
}

bool Hero::isFacingLeft() const
{
    return m_isFacingLeft;
}

HeroState& Hero::getStateManager()
{
    return m_stateManager;
}

void Hero::knockBack(const sf::Vector2f& pos, float force)
{
    auto renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!renderer)
        return;

    sf::Vector2f heroPos = renderer->getPosition();

    sf::Vector2f direction = heroPos - pos;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0)
    {
        direction.x /= length;
        direction.y /= length;
    }
    else
        direction = sf::Vector2f(0, -1);

    m_knockBack = direction * force;
    m_knockBackDuration = 0.2f;
}
