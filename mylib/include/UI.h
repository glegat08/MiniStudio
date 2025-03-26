#pragma once

#include "Composite.h"
#include "Hero.h"
#include <SFML/Graphics.hpp>

class Game;

enum class HeroIconState
{
    Neutral,
    Damaged,
    Dead
};

class HealthUI : public CompositeGameObject
{
public:
    HealthUI(const std::string& name = "HealthUI");
    ~HealthUI() = default;

    void initialize(Hero* hero, float scale = 0.5f);
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void updateIconState();

private:
    void updateHealthBar();
    void loadTextures();
    void changeHeroIcon(HeroIconState state);

private:
    Hero* m_hero;
    HeroIconState m_currentIconState;

    sf::Sprite m_healthBarBackground;
    sf::RectangleShape m_healthBarFill;
    sf::RectangleShape m_healthBarDamage;

    sf::Vector2f m_position;
    sf::Vector2f m_healthBarSize;
    sf::Vector2f m_healthBarOffset;

    float m_scale;

    sf::Sprite m_heroIcon;
    std::vector<sf::IntRect> m_iconFrames;

    bool m_isDamageAnimationPlaying;
    float m_damageAnimationTimer;
    float m_damageAnimationDuration;

    float m_targetHealthPercentage;
    float m_currentHealthPercentage;
    float m_damagePercentage;

    int m_lastHealth;
};


class ScoreUI : public CompositeGameObject
{
public:
    ScoreUI(const std::string& name = "ScoreUI");
    ~ScoreUI() = default;

    void initialize(Game* game, float scale = 0.5f);
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

private:
    void loadFont();
    void updateScoreText();

private:
    Game* m_game;
    sf::Text m_scoreText;
    sf::Font m_font;
    sf::Vector2f m_position;
    float m_scale;
    int m_lastScore;
};