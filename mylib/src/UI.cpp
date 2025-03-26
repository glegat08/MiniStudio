#include "UI.h"

#include <iostream>

#include "PathManager.h"
#include "TextureManager.h"
#include "Game.h"

HealthUI::HealthUI(const std::string& name)
    : CompositeGameObject(name)
    , m_hero(nullptr)
    , m_currentIconState(HeroIconState::Neutral)
    , m_healthBarSize(200.f, 20.f)
    , m_healthBarOffset(75.f, 10.f)
    , m_isDamageAnimationPlaying(false)
    , m_damageAnimationTimer(0.f)
    , m_damageAnimationDuration(0.5f)
    , m_targetHealthPercentage(1.0f)
    , m_currentHealthPercentage(1.0f)
    , m_damagePercentage(1.0f)
    , m_lastHealth(100)
    , m_scale(0.5f)
{
    setCategory("UI");
}

void HealthUI::initialize(Hero* hero, float scale)
{
    if (!hero)
        return;

    m_hero = hero;
    m_lastHealth = m_hero->getHealth();
    m_scale = scale;

    loadTextures();

    m_healthBarSize.x = m_healthBarSize.x * 0.30f;
    m_healthBarSize.y = m_healthBarSize.y * 0.12f;

    m_healthBarFill.setSize(sf::Vector2f(m_healthBarSize.x, m_healthBarSize.y));
    m_healthBarFill.setFillColor(sf::Color(50, 255, 50, 230));

    m_healthBarDamage.setSize(sf::Vector2f(m_healthBarSize.x, m_healthBarSize.y));
    m_healthBarDamage.setFillColor(sf::Color(255, 0, 0, 150));

    m_healthBarBackground.setScale(m_scale * 0.4f, m_scale * 0.4f);

    setPosition(sf::Vector2f(10.f, 10.f));
    changeHeroIcon(HeroIconState::Neutral);
}

void HealthUI::loadTextures()
{
    sf::Texture* originalBarTexture = TextureManager::getInstance().getTexture("health_bar_template");
    if (originalBarTexture)
    {
        sf::Vector2u originalSize = originalBarTexture->getSize();

        sf::RenderTexture renderTexture;
        if (renderTexture.create(originalSize.x / 4, originalSize.y / 4))
        {
            renderTexture.clear(sf::Color::Transparent);

            sf::Sprite tempSprite(*originalBarTexture);
            tempSprite.setScale(0.25f, 0.25f);
            renderTexture.draw(tempSprite);
            renderTexture.display();
        }
    }

    sf::Texture* barTexture = TextureManager::getInstance().getTexture("health_bar_template");
    if (barTexture)
    {
        m_healthBarBackground.setTexture(*barTexture);

        sf::Vector2u textureSize = barTexture->getSize();
        m_healthBarSize.x = static_cast<float>(textureSize.x - m_healthBarOffset.x * 0.5f);
        m_healthBarSize.y = static_cast<float>(textureSize.y - m_healthBarOffset.y * 0.5f);

        m_healthBarBackground.setOrigin(0, textureSize.y / 2.0f);
    }

    sf::Texture* iconsTexture = TextureManager::getInstance().getTexture("hero_icons");
    if (iconsTexture)
    {
        m_heroIcon.setTexture(*iconsTexture);

        sf::Vector2u textureSize = iconsTexture->getSize();
        int iconSize = textureSize.y;

        m_iconFrames.clear();
        m_iconFrames.push_back(sf::IntRect(0, 0, iconSize, iconSize)); // Neutral
        m_iconFrames.push_back(sf::IntRect(iconSize, 0, iconSize, iconSize)); // Damaged
        m_iconFrames.push_back(sf::IntRect(iconSize * 2, 0, iconSize, iconSize)); // Dead

        m_heroIcon.setTextureRect(m_iconFrames[0]);

        m_heroIcon.setOrigin(iconSize / 2.0f, iconSize / 2.0f);
        m_heroIcon.setScale(m_scale * 2.0f, m_scale * 2.0f);
    }

    m_heroIcon.setScale(m_scale * 3.5f, m_scale * 3.5f);
}

void HealthUI::update(const float& deltaTime)
{
    if (!m_hero)
        return;

    int currentHealth = m_hero->getHealth();

    if (currentHealth < m_lastHealth)
    {
        m_isDamageAnimationPlaying = true;
        m_damageAnimationTimer = 0.f;
        changeHeroIcon(HeroIconState::Damaged);
    }

    if (m_isDamageAnimationPlaying)
    {
        m_damageAnimationTimer += deltaTime;
        if (m_damageAnimationTimer >= m_damageAnimationDuration)
        {
            m_isDamageAnimationPlaying = false;
            updateIconState();
        }
    }

    m_lastHealth = currentHealth;

    updateHealthBar();

    if (!m_hero->isAlive() && m_currentIconState != HeroIconState::Dead)
        changeHeroIcon(HeroIconState::Dead);
    else if (m_hero->isAlive() && !m_isDamageAnimationPlaying && m_currentIconState != HeroIconState::Neutral)
        updateIconState();

    CompositeGameObject::update(deltaTime);
}

void HealthUI::render(sf::RenderWindow& window)
{
    window.draw(m_healthBarDamage);
    window.draw(m_healthBarFill);
    window.draw(m_healthBarBackground);
    window.draw(m_heroIcon);

    CompositeGameObject::render(window);
}

void HealthUI::setPosition(const sf::Vector2f& position)
{
    m_position = position;

    m_healthBarBackground.setPosition
	(
        m_position.x + (m_heroIcon.getGlobalBounds().width * 0.4f),
        m_position.y + m_healthBarBackground.getGlobalBounds().height / 2.0f
    );

    float scaledWidth = m_healthBarSize.x * m_scale;
    float scaledHeight = m_healthBarSize.y * m_scale;

    float healthBarX = m_position.x + (m_healthBarOffset.x * m_scale) + (m_heroIcon.getGlobalBounds().width * 0.4f);
    float healthBarY = m_position.y + (m_healthBarBackground.getGlobalBounds().height / 2.0f) - (scaledHeight / 2.0f);

    m_healthBarFill.setPosition(healthBarX + 35, healthBarY + 15);
    m_healthBarDamage.setPosition(healthBarX + 35, healthBarY + 15);

    m_healthBarFill.setSize(sf::Vector2f(scaledWidth, scaledHeight));
    m_healthBarDamage.setSize(sf::Vector2f(scaledWidth, scaledHeight));

    float iconX = m_position.x + (m_healthBarBackground.getGlobalBounds().width * 0.05f);
    float iconY = m_position.y + (m_healthBarBackground.getGlobalBounds().height / 2.0f);

    m_heroIcon.setPosition(iconX + 43, iconY);
}

sf::Vector2f HealthUI::getPosition() const
{
    return m_position;
}

void HealthUI::updateHealthBar()
{
    if (!m_hero)
        return;

    float newHealthPercentage = m_hero->getHealthPercentage();

    float scaledWidth = m_healthBarSize.x * m_scale;
    float scaledHeight = m_healthBarSize.y * m_scale;

    if (newHealthPercentage < m_targetHealthPercentage)
    {
        m_damagePercentage = m_targetHealthPercentage;
        m_healthBarDamage.setSize(sf::Vector2f(scaledWidth * m_damagePercentage, scaledHeight));
    }

    m_targetHealthPercentage = newHealthPercentage;

    float smoothness = 0.1f;
    m_currentHealthPercentage += (m_targetHealthPercentage - m_currentHealthPercentage) * smoothness;

    if (m_damagePercentage > m_targetHealthPercentage)
    {
        m_damagePercentage -= 0.005f;
        if (m_damagePercentage < m_targetHealthPercentage)
            m_damagePercentage = m_targetHealthPercentage;

        m_healthBarDamage.setSize(sf::Vector2f(scaledWidth * m_damagePercentage, scaledHeight));
    }

    m_healthBarFill.setSize(sf::Vector2f(scaledWidth * m_currentHealthPercentage, scaledHeight));

    if (m_currentHealthPercentage > 0.6f)
        m_healthBarFill.setFillColor(sf::Color(40, 230, 40, 230));
    else if (m_currentHealthPercentage > 0.3f)
        m_healthBarFill.setFillColor(sf::Color(230, 230, 40, 230));
    else
        m_healthBarFill.setFillColor(sf::Color(230, 60, 40, 230));
}

void HealthUI::updateIconState()
{
    if (!m_hero)
        return;

    if (!m_hero->isAlive())
    {
        if (m_currentIconState != HeroIconState::Dead)
            changeHeroIcon(HeroIconState::Dead);
    }
    else if (!m_isDamageAnimationPlaying && m_currentIconState != HeroIconState::Neutral)
        changeHeroIcon(HeroIconState::Neutral);
}

void HealthUI::changeHeroIcon(HeroIconState state)
{
    m_currentIconState = state;

    if (m_iconFrames.size() < 3)
        return;

    sf::Vector2f currentScale = m_heroIcon.getScale();

    switch (state)
    {
    case HeroIconState::Neutral:
        m_heroIcon.setTextureRect(m_iconFrames[0]);
        break;
    case HeroIconState::Dead:
        m_heroIcon.setTextureRect(m_iconFrames[1]);
        break;
    case HeroIconState::Damaged:
        m_heroIcon.setTextureRect(m_iconFrames[2]);
        break;
    }

    m_heroIcon.setScale(currentScale);
}



ScoreUI::ScoreUI(const std::string& name)
    : CompositeGameObject(name)
    , m_game(nullptr)
    , m_position(10.f, 50.f)
    , m_scale(0.5f)
    , m_lastScore(0)
{
    setCategory("UI");
}

void ScoreUI::initialize(Game* game, float scale)
{
    if (!game)
        return;

    m_game = game;
    m_scale = scale;
    m_lastScore = m_game->getScore();

    loadFont();

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(24);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setOutlineColor(sf::Color::Black);
    m_scoreText.setOutlineThickness(1.f);
    m_scoreText.setString("Score : 0");

    setPosition(m_position);
}

void ScoreUI::loadFont()
{
    try 
    {
        std::string fontPath = PathManager::getResourcePath("font.ttf");
        if (!m_font.loadFromFile(fontPath)) 
            std::cerr << "Failed to load font from: " << fontPath << std::endl;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error loading font: " << e.what() << std::endl;
    }
}

void ScoreUI::update(const float& deltaTime)
{
    if (!m_game)
        return;

    int currentScore = m_game->getScore();

    if (currentScore != m_lastScore)
    {
        m_lastScore = currentScore;
        updateScoreText();
    }

    CompositeGameObject::update(deltaTime);
}

void ScoreUI::render(sf::RenderWindow& window)
{
    window.draw(m_scoreText);
    CompositeGameObject::render(window);
}

void ScoreUI::setPosition(const sf::Vector2f& position)
{
    m_position = position;
    m_scoreText.setPosition(m_position);
}

sf::Vector2f ScoreUI::getPosition() const
{
    return m_position;
}

void ScoreUI::updateScoreText()
{
    m_scoreText.setString("Score: " + std::to_string(m_lastScore));
}