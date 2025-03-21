#include "Effect.h"

#include <iostream>

#include "Animation.h"
#include "TextureManager.h"
#include "Game.h"

BloodEffect::BloodEffect(const std::string& name)
    : CompositeGameObject(name)
{
    setCategory("Effect");
}

void BloodEffect::initialize(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    m_direction = direction;

    auto square_renderer = std::make_shared<SquareRenderer>(32.0f, sf::Color::Transparent);
    addComponent(square_renderer);
    square_renderer->setPosition(position);

    auto animation_component = std::make_shared<AnimationComponent>();
    addComponent(animation_component);

    const sf::Vector2i frameSize(16, 16);
    const int frameCount = 5;

    Animation bloodAnimation("blood", frameCount, 0.08f, false);
    bloodAnimation.setFrameSize(frameSize);
    bloodAnimation.setStartPosition(sf::Vector2i(0, 0));

    animation_component->addAnimation("blood", bloodAnimation);
    animation_component->playAnimation("blood");

    float angle = atan2f(direction.y, direction.x) * 180.0f / 3.14159f;
    animation_component->setScale(sf::Vector2f(4.0f, 4.0f));

    angle += 90.0f;

    auto& sprite = animation_component->m_sprite;
    sprite.setRotation(angle);

    updateAnimation();
}

void BloodEffect::update(const float& deltaTime)
{
    auto self_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (self_renderer)
    {
        sf::Vector2f parentPos = self_renderer->getPosition();
        self_renderer->setPosition(parentPos);

        updateAnimation();
    }

    CompositeGameObject::update(deltaTime);
}

bool BloodEffect::isFinished() const
{
    auto animation_component = static_cast<const AnimationComponent*>(getComponent("AnimationComponent"));
    return animation_component && animation_component->isAnimationFinished();
}

std::shared_ptr<BloodEffect> BloodEffect::createBloodEffect(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    auto bloodEffect = std::make_shared<BloodEffect>();
    bloodEffect->initialize(position, direction);

    if (Game::getInstance())
        Game::getInstance()->addEffect(bloodEffect);

    return bloodEffect;
}

void BloodEffect::updateAnimation()
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    auto animation_component = static_cast<AnimationComponent*>(getComponent("AnimationComponent"));

    if (square_renderer && animation_component)
    {
        sf::Vector2f position = square_renderer->getPosition();
        animation_component->updatePosition(position);
    }
}
