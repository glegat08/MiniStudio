#pragma once
#include "SceneBase.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class SceneManager;

class Button
{
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text,
        const sf::Font& font, unsigned int fontSize = 24);
    ~Button() = default;

    void setFunction(const std::function<void()>& function);
    void update(const sf::Vector2f& mousePosition);
    void render(sf::RenderWindow& window);
    bool isClicked(const sf::Vector2f& mousePosition);

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    void setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked);
    void setOutlineColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked);
    void setTextColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked);

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::function<void()> m_function;

    sf::Color m_normalColor;
    sf::Color m_hoverColor;
    sf::Color m_clickedColor;

    sf::Color m_normalOutlineColor;
    sf::Color m_hoverOutlineColor;
    sf::Color m_clickedOutlineColor;

    sf::Color m_normalTextColor;
    sf::Color m_hoverTextColor;
    sf::Color m_clickedTextColor;

    bool m_isHovered;
    bool m_isClicked;
};

class MainMenu : public SceneBase
{
public:
    MainMenu(sf::RenderWindow* window, const float& framerate, SceneManager* sceneManager);
    ~MainMenu() = default;

    void processInput(const sf::Event& event) override;
    void update(const float& deltaTime) override;
    void render() override;

    void initializeButtons();
    void loadResources();

private:
    sf::Font m_font;
    sf::Sprite m_logoSprite;
    sf::Texture m_logoTexture;
    sf::RectangleShape m_background;

    std::vector<Button> m_buttons;
    SceneManager* m_sceneManager;
};

class PauseMenu : public SceneBase
{
public:
    PauseMenu(sf::RenderWindow* window, const float& framerate, SceneManager* sceneManager);
    ~PauseMenu() = default;

    void processInput(const sf::Event& event) override;
    void update(const float& deltaTime) override;
    void render() override;

    void initializeButtons();
    void loadResources();

private:
    sf::Font m_font;
    sf::Text m_pauseText;
    sf::RectangleShape m_background;

    std::vector<Button> m_buttons;
    SceneManager* m_sceneManager;
};