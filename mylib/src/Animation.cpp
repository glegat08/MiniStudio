#include "Animation.h"
#include "TextureManager.h"
#include "Composite.h"

Animation::Animation()
    : m_frameCount(1)
    , m_frameTime(0.1f)
    , m_currentTime(0.0f)
    , m_currentFrame(0)
    , m_looping(true)
    , m_finished(false)
    , m_frameSize(32, 32)
    , m_startPosition(0, 0)
{
}

Animation::Animation(const std::string& textureName, int frameCount, float frameTime, bool looping)
    : m_textureName(textureName)
    , m_frameCount(frameCount)
    , m_frameTime(frameTime)
    , m_currentTime(0.0f)
    , m_currentFrame(0)
    , m_looping(looping)
    , m_finished(false)
    , m_frameSize(32, 32)
    , m_startPosition(0, 0)
{
}

void Animation::update(float deltaTime)
{
    if (m_finished && !m_looping)
        return;

    m_currentTime += deltaTime;

    if (m_currentTime >= m_frameTime)
    {
        m_currentTime = 0.0f;
        m_currentFrame++;

        if (m_currentFrame >= m_frameCount)
        {
            if (m_looping)
                m_currentFrame = 0;
            else
            {
                m_currentFrame = m_frameCount - 1;
                m_finished = true;
            }
        }
    }
}

void Animation::reset()
{
    m_currentFrame = 0;
    m_currentTime = 0.0f;
    m_finished = false;
}

void Animation::setTexture(const std::string& textureName)
{
    m_textureName = textureName;
}

void Animation::setFrameCount(int frameCount)
{
    m_frameCount = frameCount;
}

void Animation::setFrameTime(float frameTime)
{
    m_frameTime = frameTime;
}

void Animation::setLooping(bool looping)
{
    m_looping = looping;
}

void Animation::setFrameSize(const sf::Vector2i& frameSize)
{
    m_frameSize = frameSize;
}

void Animation::setStartPosition(const sf::Vector2i& startPos)
{
    m_startPosition = startPos;
}

bool Animation::isFinished() const
{
    return m_finished;
}

int Animation::getCurrentFrame() const
{
    return m_currentFrame;
}

sf::IntRect Animation::getCurrentFrameRect() const
{
    int x = m_startPosition.x + (m_currentFrame * m_frameSize.x);
    int y = m_startPosition.y;

    return { x, y, m_frameSize.x, m_frameSize.y };
}

AnimationComponent::AnimationComponent()
    : Component("AnimationComponent")
    , m_currentAnimation("")
    , m_scale(1.0f, 1.0f)
    , m_playing(false)
{
}

void AnimationComponent::initialize()
{
    Component::initialize();
}

void AnimationComponent::update(const float& deltaTime)
{
    if (!m_playing || m_currentAnimation.empty())
        return;

    auto& animation = m_animations[m_currentAnimation];
    animation.update(deltaTime);

    m_sprite.setTextureRect(animation.getCurrentFrameRect());
}

void AnimationComponent::render(sf::RenderWindow& window)
{
    if (!m_playing || m_currentAnimation.empty())
        return;

    window.draw(m_sprite);
}

void AnimationComponent::addAnimation(const std::string& name, const Animation& animation)
{
    m_animations[name] = animation;
}

void AnimationComponent::playAnimation(const std::string& name)
{
    if (m_animations.find(name) == m_animations.end())
        return;

    if (m_currentAnimation == name && m_playing)
        return;

    sf::Vector2f currentPosition = m_sprite.getPosition();
    sf::Vector2f currentScale = m_sprite.getScale();

    m_currentAnimation = name;
    m_animations[name].reset();

    sf::Texture* texture = TextureManager::getInstance().getTexture(m_animations[name].m_textureName);
    if (texture)
    {
        m_sprite.setTexture(*texture);
        m_sprite.setTextureRect(m_animations[name].getCurrentFrameRect());
    }

    m_sprite.setPosition(currentPosition);
    m_sprite.setScale(currentScale);

    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

    m_playing = true;
}

void AnimationComponent::stopAnimation()
{
    m_playing = false;
}

void AnimationComponent::updatePosition(const sf::Vector2f& position)
{
    sf::Vector2f currentScale = m_sprite.getScale();

    m_sprite.setPosition(position);

    m_sprite.setScale(currentScale);
}

void AnimationComponent::setScale(const sf::Vector2f& scale)
{
    m_scale = scale;
    m_sprite.setScale(scale);
}

sf::Vector2f AnimationComponent::getScale() const
{
    return m_scale;
}

bool AnimationComponent::isAnimationFinished() const
{
    if (m_currentAnimation.empty())
        return true;

    return m_animations.at(m_currentAnimation).isFinished();
}

std::string AnimationComponent::getCurrentAnimation() const
{
    return m_currentAnimation;
}