#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Component.h"
#include "TextureManager.h"

class Animation
{
public:
    Animation();
    Animation(const std::string& textureName, int frameCount, float frameTime, bool looping = true);

    void update(float deltaTime);
    void reset();

    void setTexture(const std::string& textureName);
    void setFrameCount(int frameCount);
    void setFrameTime(float frameTime);
    void setLooping(bool looping);
    void setFrameSize(const sf::Vector2i& frameSize);
    void setStartPosition(const sf::Vector2i& startPos);

    bool isFinished() const;
    int getCurrentFrame() const;
    sf::IntRect getCurrentFrameRect() const;

    std::string m_textureName;

private:
    int m_frameCount;
    float m_frameTime;
    float m_currentTime;
    int m_currentFrame;
    bool m_looping;
    bool m_finished;
    sf::Vector2i m_frameSize;
    sf::Vector2i m_startPosition;
};

class AnimationComponent : public Component
{
public:
    AnimationComponent();
    ~AnimationComponent() override = default;

    void initialize() override;
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void addAnimation(const std::string& name, const Animation& animation);
    void playAnimation(const std::string& name);
    void stopAnimation();

    void updatePosition(const sf::Vector2f& position);
    void setScale(const sf::Vector2f& scale);
    sf::Vector2f getScale() const;

    bool isAnimationFinished() const;
    std::string getCurrentAnimation() const;

private:
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_currentAnimation;
    sf::Sprite m_sprite;
    sf::Vector2f m_scale;
    bool m_playing;
};