#include "Hero.h"
#include "TextureManager.h"
#include "Component.h"
#include "Animation.h"

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

    sf::Color collisionColor = color;
    collisionColor.a = 50;
    renderer->setColor(collisionColor);

    auto controller = std::make_shared<PlayerController>(speed);
    addComponent(controller);

    auto animComp = std::make_shared<AnimationComponent>();
    addComponent(animComp);

    m_speed = speed;

    loadTextures();
    initializeAnimations();

    m_stateManager.initialize();
}

void Hero::loadTextures()
{
    TextureManager& texManager = TextureManager::getInstance();

    if (!texManager.hasTexture("player")) 
        texManager.loadTexture("player", "hero\\player.png");
}

void Hero::initializeAnimations()
{
    auto animComp = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));
    if (!animComp) 
        return;

    const sf::Vector2i frameSize(32, 32);

    const int idleFrameCount = 2;
    const int runFrameCount = 2;
    const int shootFrameCount = 4;
    const int attackFrameCount = 6;
    const int hurtFrameCount = 1;
    const int deathFrameCount = 18;

    const int downRow = 0;
    const int rightRow = 1;
    const int upRow = 2;
    const int deathRow = 3;
    const int hurtRow = 4;

    const int idleOffset = 0;
    const int runOffset = idleFrameCount;
    const int shootOffset = idleFrameCount + runFrameCount;
    const int attackOffset = idleFrameCount + runFrameCount + shootFrameCount;

    // DOWN direction animations
    Animation idleDownAnim("player", idleFrameCount, 0.4f);
    idleDownAnim.setFrameSize(frameSize);
    idleDownAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, downRow * frameSize.y));
    animComp->addAnimation("idle_down", idleDownAnim);

    Animation runDownAnim("player", runFrameCount, 0.2f);
    runDownAnim.setFrameSize(frameSize);
    runDownAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, downRow * frameSize.y));
    animComp->addAnimation("run_down", runDownAnim);

    Animation shootDownAnim("player", shootFrameCount, 0.1f, false);
    shootDownAnim.setFrameSize(frameSize);
    shootDownAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, downRow * frameSize.y));
    animComp->addAnimation("shoot_down", shootDownAnim);

    Animation attackDownAnim("player", attackFrameCount, 0.1f, false);
    attackDownAnim.setFrameSize(frameSize);
    attackDownAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, downRow * frameSize.y));
    animComp->addAnimation("attack_down", attackDownAnim);

    // RIGHT direction animations
    Animation idleRightAnim("player", idleFrameCount, 0.4f);
    idleRightAnim.setFrameSize(frameSize);
    idleRightAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, rightRow * frameSize.y));
    animComp->addAnimation("idle_right", idleRightAnim);

    Animation runRightAnim("player", runFrameCount, 0.2f);
    runRightAnim.setFrameSize(frameSize);
    runRightAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, rightRow * frameSize.y));
    animComp->addAnimation("run_right", runRightAnim);

    Animation shootRightAnim("player", shootFrameCount, 0.1f, false);
    shootRightAnim.setFrameSize(frameSize);
    shootRightAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, rightRow * frameSize.y));
    animComp->addAnimation("shoot_right", shootRightAnim);

    Animation attackRightAnim("player", attackFrameCount, 0.1f, false);
    attackRightAnim.setFrameSize(frameSize);
    attackRightAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, rightRow * frameSize.y));
    animComp->addAnimation("attack_right", attackRightAnim);

    // UP direction animations
    Animation idleUpAnim("player", idleFrameCount, 0.4f);
    idleUpAnim.setFrameSize(frameSize);
    idleUpAnim.setStartPosition(sf::Vector2i(idleOffset * frameSize.x, upRow * frameSize.y));
    animComp->addAnimation("idle_up", idleUpAnim);

    Animation runUpAnim("player", runFrameCount, 0.2f);
    runUpAnim.setFrameSize(frameSize);
    runUpAnim.setStartPosition(sf::Vector2i(runOffset * frameSize.x, upRow * frameSize.y));
    animComp->addAnimation("run_up", runUpAnim);

    Animation shootUpAnim("player", shootFrameCount, 0.1f, false);
    shootUpAnim.setFrameSize(frameSize);
    shootUpAnim.setStartPosition(sf::Vector2i(shootOffset * frameSize.x, upRow * frameSize.y));
    animComp->addAnimation("shoot_up", shootUpAnim);

    Animation attackUpAnim("player", attackFrameCount, 0.1f, false);
    attackUpAnim.setFrameSize(frameSize);
    attackUpAnim.setStartPosition(sf::Vector2i(attackOffset * frameSize.x, upRow * frameSize.y));
    animComp->addAnimation("attack_up", attackUpAnim);

    // HURT animation (single row)
    Animation hurtAnim("player", hurtFrameCount, 0.15f, false);
    hurtAnim.setFrameSize(frameSize);
    hurtAnim.setStartPosition(sf::Vector2i(0, hurtRow * frameSize.y));
    animComp->addAnimation("hurt", hurtAnim);

    // DEATH animation (single row)
    Animation deathAnim("player", deathFrameCount, 0.2f, false);
    deathAnim.setFrameSize(frameSize);
    deathAnim.setStartPosition(sf::Vector2i(0, deathRow * frameSize.y));
    animComp->addAnimation("death", deathAnim);

    // Set default animation
    animComp->playAnimation("idle_down");
}

void Hero::update(const float& deltaTime)
{
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
    if (actualDamage < 1) actualDamage = 1;

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

    std::string dirSuffix;
    switch (m_currentDirection)
	{
    case Direction::Up:
        dirSuffix = "_up";
        break;
    case Direction::Right:
    case Direction::Left:
        dirSuffix = "_right";
        break;
    case Direction::Down:
    default:
        dirSuffix = "_down";
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
        animComp->playAnimation("idle" + dirSuffix);
        break;
    case stateName::run:
        animComp->playAnimation("run" + dirSuffix);
        break;
    case stateName::attack:
        animComp->playAnimation("attack" + dirSuffix);
        break;
    case stateName::shoot:
        animComp->playAnimation("shoot" + dirSuffix);
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