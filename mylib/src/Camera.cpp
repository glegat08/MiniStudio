#include "Camera.h"
#include <cmath>
#include <algorithm>

CameraTargetAdapter::CameraTargetAdapter(CompositeGameObject* owner)
    : m_owner(owner)
{
}

sf::Vector2f CameraTargetAdapter::getPosition() const
{
    if (!m_owner)
        return sf::Vector2f(0.f, 0.f);

    auto* renderer = static_cast<SquareRenderer*>(m_owner->getComponent("SquareRenderer"));
    if (renderer)
    {
        return renderer->getPosition();
    }

    return sf::Vector2f(0.f, 0.f);
}

sf::Vector2f CameraTargetAdapter::getVelocity() const
{
    if (!m_owner)
        return sf::Vector2f(0.f, 0.f);

    auto* controller = static_cast<PlayerController*>(m_owner->getComponent("PlayerController"));
    if (controller)
    {
        sf::Vector2f velocity(0.f, 0.f);
        float speed = controller->getSpeed();

        if (controller->m_isMovingRight) 
            velocity.x += 1.0f;
        if (controller->m_isMovingLeft) 
            velocity.x -= 1.0f;
        if (controller->m_isMovingDown) 
            velocity.y += 1.0f;
        if (controller->m_isMovingUp)
            velocity.y -= 1.0f;

        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (length > 0.f)
        {
            velocity /= length;
            velocity *= speed;
        }

        return velocity;
    }

    return sf::Vector2f(0.f, 0.f);
}

Camera::Camera()
    : m_window(nullptr),
    m_target(nullptr),
    m_following(false),
    m_targetPosition(0.f, 0.f),
    m_currentPosition(0.f, 0.f),
    m_hasBounds(false),
    m_zoom(1.0f),
    m_rotation(0.0f),
    m_interpolationSpeed(5.0f),
    m_lookAheadFactor(0.2f),
    m_velocity(0.f, 0.f)
{
}

void Camera::initialize(sf::RenderWindow* window)
{
    m_window = window;

    if (m_window)
    {
        m_defaultView = m_window->getDefaultView();
        m_view = m_defaultView;
    }
}

void Camera::update(const float& deltaTime)
{
    if (!m_window)
        return;

    if (m_following && m_target)
    {
        m_targetPosition = m_target->getPosition();

        sf::Vector2f velocity = m_target->getVelocity();

        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        if (length > 0.f)
            m_targetPosition += velocity * m_lookAheadFactor;
    }

    updateInterpolation(deltaTime);

    sf::Vector2f finalPosition = m_currentPosition;
    m_view.setCenter(finalPosition);

    m_view.setSize(m_defaultView.getSize() / m_zoom);
    m_view.setRotation(m_rotation);

    if (m_hasBounds)
        applyWorldBounds();
}

void Camera::apply()
{
    if (m_window)
        m_window->setView(m_view);
}

void Camera::setCenter(const sf::Vector2f& center)
{
    m_targetPosition = center;
    m_following = false;
    m_target = nullptr;
}

void Camera::follow(ICameraTarget* target)
{
    if (target)
    {
        m_target = target;
        m_following = true;

        m_targetPosition = target->getPosition();
        m_currentPosition = m_targetPosition;
    }
}

void Camera::stopFollowing()
{
    m_following = false;
    m_target = nullptr;
}

void Camera::setWorldBounds(const sf::FloatRect& bounds)
{
    m_worldBounds = bounds;
    m_hasBounds = true;
}

void Camera::setZoom(float zoom)
{
    m_zoom = std::max(0.1f, zoom);
}

float Camera::getZoom() const
{
    return m_zoom;
}

void Camera::setRotation(float angle)
{
    m_rotation = angle;
}

float Camera::getRotation() const
{
    return m_rotation;
}

void Camera::setInterpolationSpeed(float speed)
{
    m_interpolationSpeed = std::max(0.1f, speed);
}

void Camera::setLookAheadFactor(float factor)
{
    m_lookAheadFactor = factor;
}

sf::View& Camera::getView()
{
    return m_view;
}

const sf::View& Camera::getView() const
{
    return m_view;
}

sf::Vector2f Camera::getOffset() const
{
    return m_currentPosition - m_view.getCenter();
}

sf::FloatRect Camera::getVisibleArea() const
{
    sf::Vector2f center = m_view.getCenter();
    sf::Vector2f size = m_view.getSize();

    return sf::FloatRect
    (
        center.x - size.x / 2.f,
        center.y - size.y / 2.f,
        size.x,
        size.y
    );
}

void Camera::applyWorldBounds()
{
    sf::Vector2f viewSize = m_view.getSize();
    sf::Vector2f viewCenter = m_view.getCenter();

    float viewLeft = viewCenter.x - viewSize.x / 2.f;
    float viewTop = viewCenter.y - viewSize.y / 2.f;
    float viewRight = viewCenter.x + viewSize.x / 2.f;
    float viewBottom = viewCenter.y + viewSize.y / 2.f;

    if (viewSize.x <= m_worldBounds.width)
    {
        if (viewLeft < m_worldBounds.left)
            viewCenter.x = m_worldBounds.left + viewSize.x / 2.f;
        else if (viewRight > m_worldBounds.left + m_worldBounds.width)
            viewCenter.x = m_worldBounds.left + m_worldBounds.width - viewSize.x / 2.f;
    }
    else
        viewCenter.x = m_worldBounds.left + m_worldBounds.width / 2.f;

    if (viewSize.y <= m_worldBounds.height)
    {
        if (viewTop < m_worldBounds.top)
            viewCenter.y = m_worldBounds.top + viewSize.y / 2.f;
        else if (viewBottom > m_worldBounds.top + m_worldBounds.height)
            viewCenter.y = m_worldBounds.top + m_worldBounds.height - viewSize.y / 2.f;
    }
    else
        viewCenter.y = m_worldBounds.top + m_worldBounds.height / 2.f;

    m_view.setCenter(viewCenter);
    m_currentPosition = viewCenter;
}

void Camera::updateInterpolation(const float& deltaTime)
{
    float factor = 1.0f - std::exp(-m_interpolationSpeed * deltaTime);
    m_currentPosition += (m_targetPosition - m_currentPosition) * factor;
}


CameraTarget::CameraTarget(float weight, bool autoFollow)
    : Component("CameraTarget"),
    m_weight(weight),
    m_offset(0.f, 0.f),
    m_autoFollow(autoFollow),
    m_active(false),
    m_adapter(nullptr)
{
}

CameraTarget::~CameraTarget()
{
    if (m_active)
        deactivate();

    delete m_adapter;
}

void CameraTarget::initialize()
{
    Component::initialize();

    if (m_owner)
    {
        m_adapter = new CameraTargetAdapter(static_cast<CompositeGameObject*>(m_owner));

        if (m_autoFollow)
            activate();
    }
}

void CameraTarget::update(const float& deltaTime)
{
}

void CameraTarget::setWeight(float weight)
{
    m_weight = weight;
}

float CameraTarget::getWeight() const
{
    return m_weight;
}

void CameraTarget::setOffset(const sf::Vector2f& offset)
{
    m_offset = offset;
}

sf::Vector2f CameraTarget::getOffset() const
{
    return m_offset;
}

void CameraTarget::setAutoFollow(bool autoFollow)
{
    m_autoFollow = autoFollow;
}

bool CameraTarget::getAutoFollow() const
{
    return m_autoFollow;
}

void CameraTarget::activate()
{
    if (m_owner && m_adapter && !m_active)
    {
        Camera::getInstance().follow(m_adapter);
        m_active = true;
    }
}

void CameraTarget::deactivate()
{
    if (m_active)
    {
        Camera::getInstance().stopFollowing();
        m_active = false;
    }
}