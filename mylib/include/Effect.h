#pragma once

#include "Composite.h"
#include <SFML/Graphics.hpp>

class BloodEffect : public CompositeGameObject
{
public:
    BloodEffect(const std::string& name = "BloodEffect");
    ~BloodEffect() = default;

    void initialize(const sf::Vector2f& position, const sf::Vector2f& direction = sf::Vector2f(0, 1));
	void update(const float& deltaTime) override;

    bool isFinished() const;

    static std::shared_ptr<BloodEffect> createBloodEffect(const sf::Vector2f& position, const sf::Vector2f& direction = sf::Vector2f(0, 1));

    void updateAnimation();

private:
    sf::Vector2f m_direction;
};