#include "Hero.h"
#include "TextureManager.h"
#include "Component.h"
#include "Animation.h"
#include "AudioManager.h"
#include "Collision.h"
#include "Effect.h"
#include "Map.h"

Hero::Hero(const std::string& name)
    : CompositeGameObject(name)
    , m_health(100)
    , m_maxHealth(100)
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
    auto square_renderer = std::make_shared<SquareRenderer>(size, color);
    this->addComponent(square_renderer);
    square_renderer->setPosition(position);

    auto controller = std::make_shared<PlayerController>(speed);
    addComponent(controller);

    auto animation_component = std::make_shared<AnimationComponent>();
    addComponent(animation_component);

    auto hitbox = std::make_shared<Hitbox>(sf::Vector2f(60.f, 60.f));
    hitbox->setDebugDraw(false);
    addComponent(hitbox);

    auto sound_component = std::make_shared<SoundComponent>();
    sound_component->addSoundMapping("attack", "hero_attack");
    addComponent(sound_component);

    m_speed = speed;
    m_stateManager.initialize();
}

void Hero::update(const float& deltaTime)
{
    if (m_knockBackDuration > 0)
    {
        m_knockBackDuration -= deltaTime;

        auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
        if (square_renderer)
        {
            sf::Vector2f currentPos = square_renderer->getPosition();

            float slowdown_factor = std::min(1.0f, 4.0f * deltaTime);
            m_knockBack *= (1.0f - slowdown_factor);

			sf::Vector2f newPos = currentPos + m_knockBack * deltaTime;

            float playerRadius = 25.f;
			newPos = WorldLimits::limits(newPos, playerRadius);
            square_renderer->setPosition(currentPos + m_knockBack * deltaTime);
        }
    }

    CompositeGameObject::update(deltaTime);
    m_stateManager.update(deltaTime);
    updateAnimationPosition();
}

void Hero::updateAnimationPosition()
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (square_renderer && animation_component) 
    {
        sf::Vector2f position = square_renderer->getPosition();
        animation_component->updatePosition(position);
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

void Hero::takeDamage(int amount, const sf::Vector2f& attackerPos)
{
    int actualDamage = amount - m_armor;
    if (actualDamage < 1) 
        actualDamage = 1;

    m_health -= actualDamage;

	SoundManager::getInstance().playSound("hit", 80.f);

	auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
	if (square_renderer)
    {
        sf::Vector2f position = square_renderer->getPosition();
        sf::Vector2f entryDirection = position - attackerPos;
        sf::Vector2f bloodDirection = entryDirection;
        
        float length = std::sqrt(bloodDirection.x * bloodDirection.x + bloodDirection.y * bloodDirection.y);
        if (length > 0)
        {
            bloodDirection.x /= length;
            bloodDirection.y /= length;
        }
        else
            bloodDirection = sf::Vector2f(0, 1);
            
        BloodEffect::createBloodEffect(position, bloodDirection);
    }

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
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (square_renderer) 
    {
        sf::Vector2f currentPos = square_renderer->getPosition();
        square_renderer->setPosition(currentPos + offset);
    }
}

void Hero::setDirection(Direction direction)
{
    m_currentDirection = direction;

    if (direction == Direction::Left) 
        m_isFacingLeft = true;
    else if (direction == Direction::Right) 
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

    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animation_component)
        return;

    std::string direction_suffix;
    switch (m_currentDirection)
    {
    case Direction::Up:
        direction_suffix = "_up";
        break;
    case Direction::Down:
        direction_suffix = "_down";
        break;
    case Direction::Right:
    case Direction::Left:
        direction_suffix = "_right";
        break;
    }

    if (newState == stateName::hurt)
    {
        animation_component->playAnimation("hurt");
        return;
    }

    if (newState == stateName::death)
    {
        animation_component->playAnimation("death");
        return;
    }

    switch (newState)
    {
    case stateName::idle:
        animation_component->playAnimation("idle" + direction_suffix);
        break;
    case stateName::run:
        animation_component->playAnimation("run" + direction_suffix);
        break;
    case stateName::attack:
        animation_component->playAnimation("attack" + direction_suffix);
        break;
    case stateName::shoot:
        animation_component->playAnimation("shoot" + direction_suffix);
        break;
    default:
        break;
    }

    if (m_isFacingLeft && (m_currentDirection == Direction::Left || m_currentDirection == Direction::Right))
        animation_component->setScale(sf::Vector2f(-2.0f, 2.0f));
    else
        animation_component->setScale(sf::Vector2f(2.0f, 2.0f));
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
    if (m_currentStateName == stateName::death)
    {
        m_knockBackDuration = 0.f;
        m_knockBack = sf::Vector2f(0.f, 0.f);
        return;
    }

    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (!square_renderer)
        return;

    sf::Vector2f heroPos = square_renderer->getPosition();

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

int Hero::getHealth() const
{
    return m_health;
}

int Hero::getMaxHealth() const
{
    return m_maxHealth;
}

float Hero::getHealthPercentage() const
{
    if (m_maxHealth <= 0)
        return 0.0f;

    return static_cast<float>(m_health) / static_cast<float>(m_maxHealth);
}

void Hero::setMaxHealth(int maxHealth)
{
    m_maxHealth = maxHealth;
    if (m_health > m_maxHealth)
        m_health = m_maxHealth;
}

void Hero::heal(int amount)
{
    m_health += amount;
    if (m_health > m_maxHealth)
        m_health = m_maxHealth;
}