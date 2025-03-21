#include "Arrow.h"
#include "Animation.h"
#include "Collision.h"
#include "Hero.h"
#include "TextureManager.h"

Arrow::Arrow(const std::string& name, int damage)
    : IProjectile(name)
{
    m_damage = damage;
    m_maxTimeBeforeDestroy = 3.0f;
    addTag("Arrow");
}

Arrow::~Arrow()
{
}

void Arrow::initialize(const sf::Vector2f& pos, const sf::Vector2f& direction, float speed)
{
    auto square_renderer = std::make_shared<SquareRenderer>(8.0f, sf::Color::Transparent);
    addComponent(square_renderer);
    square_renderer->setPosition(pos);

    auto animation_component = std::make_shared<AnimationComponent>();
    addComponent(animation_component);

    const sf::Vector2i frameSize(16, 16);
    Animation arrow_animation("arrow", 1, 0.1f);
    arrow_animation.setFrameSize(frameSize);
    arrow_animation.setStartPosition(sf::Vector2i(0, 0));
    animation_component->addAnimation("arrow", arrow_animation);
    animation_component->playAnimation("arrow");
    animation_component->setScale(sf::Vector2f(4.f, 4.f));

    auto hitbox = std::make_shared<Hitbox>(sf::Vector2f(16.0f, 8.0f));
    hitbox->setDebugDraw(false);
    addComponent(hitbox);

    hitbox->setEnterCollision([this](Hitbox* self, Hitbox* other) 
        {
        auto object = other->getOwner();
        if (object && object->hasTag("Hero")) 
        {
            auto hero = dynamic_cast<Hero*>(object);
            if (hero) 
            {
                auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
                if (square_renderer) 
                {
                    sf::Vector2f arrowPos = square_renderer->getPosition();
                    hero->takeDamage(m_damage, arrowPos);
                }
                else
                    hero->takeDamage(m_damage, sf::Vector2f(0, 0));
            }
        }
        });

    m_direction = direction;
    m_speed = speed;

    updateAnimation();
}

void Arrow::update(const float& deltaTime)
{
    auto square_renderer = static_cast<SquareRenderer*>(getComponent("SquareRenderer"));
    if (square_renderer)
    {
        sf::Vector2f pos = square_renderer->getPosition();
        pos += m_direction * m_speed * deltaTime;
        square_renderer->setPosition(pos);

        updateAnimation();
    }

    IProjectile::update(deltaTime);
}

int Arrow::getDamage() const
{
    return m_damage;
}

bool Arrow::isLifeTimePassed() const
{
    return m_TimeBeforeDestroy >= m_maxTimeBeforeDestroy;
}
