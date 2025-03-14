#pragma once

#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Composite.h"
#include <functional>

class ICameraTarget
{
public:
    virtual ~ICameraTarget() = default;
    virtual sf::Vector2f getPosition() const = 0;
    virtual sf::Vector2f getVelocity() const = 0;
};

class Camera
{
public:
    static Camera& getInstance()
    {
        static Camera instance;
        return instance;
    }

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void initialize(sf::RenderWindow* window);
    void update(const float& deltaTime);
    void apply();

    void setCenter(const sf::Vector2f& center);
    void follow(ICameraTarget* target);
    void stopFollowing();

    void setWorldBounds(const sf::FloatRect& bounds);

    void setZoom(float zoom);
    float getZoom() const;

    void setRotation(float angle);
    float getRotation() const;

    void setInterpolationSpeed(float speed);
    void setLookAheadFactor(float factor);

    sf::View& getView();
    const sf::View& getView() const;
    sf::Vector2f getOffset() const;
    sf::FloatRect getVisibleArea() const;

private:
    Camera();
    ~Camera() = default;

    void applyWorldBounds();
    void updateInterpolation(const float& deltaTime);

    sf::RenderWindow* m_window;
    sf::View m_view;
    sf::View m_defaultView;

    ICameraTarget* m_target;
    bool m_following;

    sf::Vector2f m_targetPosition;
    sf::Vector2f m_currentPosition;

    sf::FloatRect m_worldBounds;
    bool m_hasBounds;

    float m_zoom;
    float m_rotation;

    float m_interpolationSpeed;
    float m_lookAheadFactor;
    sf::Vector2f m_velocity;
};

class CameraTargetAdapter : public ICameraTarget
{
public:
    CameraTargetAdapter(CompositeGameObject* owner);
    sf::Vector2f getPosition() const override;
    sf::Vector2f getVelocity() const override;

private:
    CompositeGameObject* m_owner;
};

class CameraTarget : public Component
{
public:
    CameraTarget(float weight = 1.0f, bool autoFollow = true);
    ~CameraTarget() override;

    void initialize() override;
    void update(const float& deltaTime) override;

    void setWeight(float weight);
    float getWeight() const;

    void setOffset(const sf::Vector2f& offset);
    sf::Vector2f getOffset() const;

    void setAutoFollow(bool autoFollow);
    bool getAutoFollow() const;

    void activate();
    void deactivate();

private:
    float m_weight;
    sf::Vector2f m_offset;
    bool m_autoFollow;
    bool m_active;
    CameraTargetAdapter* m_adapter;
};