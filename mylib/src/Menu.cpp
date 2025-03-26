#include "Menu.h"
#include "PathManager.h"
#include "SceneManager.h"
#include "Game.h"
#include <iostream>

#include "AudioManager.h"

// Button Implementation
Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text,
    const sf::Font& font, unsigned int fontSize)
    : m_normalColor(sf::Color(100, 100, 100))
    , m_hoverColor(sf::Color(150, 150, 150))
    , m_clickedColor(sf::Color(80, 80, 80))
    , m_normalOutlineColor(sf::Color(200, 200, 200))
    , m_hoverOutlineColor(sf::Color(255, 255, 255))
    , m_clickedOutlineColor(sf::Color(180, 180, 180))
    , m_normalTextColor(sf::Color(255, 255, 255))
    , m_hoverTextColor(sf::Color(255, 255, 0))
    , m_clickedTextColor(sf::Color(200, 200, 200))
    , m_isHovered(false)
    , m_isClicked(false)
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setFillColor(m_normalColor);
    m_shape.setOutlineColor(m_normalOutlineColor);
    m_shape.setOutlineThickness(2.0f);

    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(m_normalTextColor);

    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    m_text.setPosition
	(
        position.x + size.x / 2,
        position.y + size.y / 2 - textBounds.height / 4
    );
}

void Button::setFunction(const std::function<void()>& function)
{
    m_function = function;
}

void Button::update(const sf::Vector2f& mousePosition)
{
    m_isHovered = m_shape.getGlobalBounds().contains(mousePosition);

    bool wasHovered = m_isHovered;

    if (wasHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        m_isClicked = true;
        m_shape.setFillColor(m_clickedColor);
        m_shape.setOutlineColor(m_clickedOutlineColor);
        m_text.setFillColor(m_clickedTextColor);
    }
    else if (wasHovered)
    {
        m_shape.setFillColor(m_hoverColor);
        m_shape.setOutlineColor(m_hoverOutlineColor);
        m_text.setFillColor(m_hoverTextColor);
    }
    else
    {
        m_isClicked = false;
        m_shape.setFillColor(m_normalColor);
        m_shape.setOutlineColor(m_normalOutlineColor);
        m_text.setFillColor(m_normalTextColor);
    }
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
    window.draw(m_text);
}

bool Button::isClicked(const sf::Vector2f& mousePosition)
{
    if (m_shape.getGlobalBounds().contains(mousePosition) &&
        m_isClicked && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        m_isClicked = false;
        if (m_function)
        {
            m_function();
            return true;
        }
    }
    return false;
}

void Button::setPosition(const sf::Vector2f& position)
{
    m_shape.setPosition(position);

    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setPosition
	(
        position.x + m_shape.getSize().x / 2,
        position.y + m_shape.getSize().y / 2 - textBounds.height / 4
    );
}

sf::Vector2f Button::getPosition() const
{
    return m_shape.getPosition();
}

sf::Vector2f Button::getSize() const
{
    return m_shape.getSize();
}

void Button::setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked)
{
    m_normalColor = normal;
    m_hoverColor = hover;
    m_clickedColor = clicked;

    m_shape.setFillColor(m_normalColor);
}

void Button::setOutlineColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked)
{
    m_normalOutlineColor = normal;
    m_hoverOutlineColor = hover;
    m_clickedOutlineColor = clicked;

    m_shape.setOutlineColor(m_normalOutlineColor);
}

void Button::setTextColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& clicked)
{
    m_normalTextColor = normal;
    m_hoverTextColor = hover;
    m_clickedTextColor = clicked;

    m_text.setFillColor(m_normalTextColor);
}


// MainMenu Implementation
MainMenu::MainMenu(sf::RenderWindow* window, const float& framerate, SceneManager* sceneManager)
    : SceneBase(window, framerate, "MainMenu")
    , m_sceneManager(sceneManager)
{
    loadResources();
    initializeButtons();

    m_background.setSize(sf::Vector2f(window->getSize()));
    m_background.setFillColor(sf::Color(30, 30, 30, 200));
}

void MainMenu::loadResources()
{
    try
    {
        SoundManager::getInstance().loadMusic("game_music", "music\\game_music.wav");
        SoundManager::getInstance().playMusic("game_music", 20.0f);
        SoundManager::getInstance().setMusicLoop(true);

        std::string fontPath = PathManager::getResourcePath("font.ttf");
        if (!m_font.loadFromFile(fontPath))
        {
            std::cerr << "Failed to load font from: " << fontPath << std::endl;
        }

        std::string logoPath = PathManager::getResourcePath("logo.png");
        if (!m_logoTexture.loadFromFile(logoPath))
        {
            std::cerr << "Failed to load logo from: " << logoPath << std::endl;
        }
        else
        {
            m_logoSprite.setTexture(m_logoTexture);

            sf::FloatRect logoBounds = m_logoSprite.getLocalBounds();
            m_logoSprite.setOrigin(logoBounds.width / 2, 0);
            m_logoSprite.setPosition(m_renderWindow->getSize().x / 2, 50);

            float maxWidth = m_renderWindow->getSize().x * 0.7f;
            if (logoBounds.width > maxWidth)
            {
                float scale = maxWidth / logoBounds.width;
                m_logoSprite.setScale(scale, scale);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading resources: " << e.what() << std::endl;
    }
}

void MainMenu::initializeButtons()
{
    const float buttonWidth = 200.0f;
    const float buttonHeight = 60.0f;
    const float buttonSpacing = 30.0f;
    const float startY = (m_renderWindow->getSize().y / 2) + 200;

    Button playButton
	(
        sf::Vector2f((m_renderWindow->getSize().x - buttonWidth) / 2, startY),
        sf::Vector2f(buttonWidth, buttonHeight),
        "P L A Y",
        m_font,
        30
    );

    playButton.setFunction([this]() 
        {
    	SoundManager::getInstance().stopMusic();

        SoundManager::getInstance().playMusic("game_on", 20.0f);
        m_sceneManager->setCurrentScene("Game");
        });

    Button quitButton
	(
        sf::Vector2f((m_renderWindow->getSize().x - buttonWidth) / 2, startY + buttonHeight + buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        "Q U I T",
        m_font,
        30
    );

    quitButton.setFunction([this]() 
        {
        m_renderWindow->close();
        });

    m_buttons.push_back(playButton);
    m_buttons.push_back(quitButton);
}

void MainMenu::processInput(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePosition = m_renderWindow->mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        for (auto& button : m_buttons)
        {
            button.isClicked(mousePosition);
        }
    }

    SceneBase::processInput(event);
}

void MainMenu::update(const float& deltaTime)
{
    sf::Vector2f mousePosition = m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));

    for (auto& button : m_buttons)
    {
        button.update(mousePosition);
    }

    SceneBase::update(deltaTime);
}

void MainMenu::render()
{
    m_renderWindow->draw(m_background);
    m_renderWindow->draw(m_logoSprite);

    for (auto& button : m_buttons)
    {
        button.render(*m_renderWindow);
    }

    SceneBase::render();
}



// PauseMenu Implementation
PauseMenu::PauseMenu(sf::RenderWindow* window, const float& framerate, SceneManager* sceneManager)
    : SceneBase(window, framerate, "Pause")
    , m_sceneManager(sceneManager)
{
    loadResources();
    initializeButtons();

    m_background.setSize(sf::Vector2f(window->getSize()));
    m_background.setFillColor(sf::Color(0, 0, 0, 180));
}

void PauseMenu::loadResources()
{
    try
    {
        std::string fontPath = PathManager::getResourcePath("font.ttf");
        if (!m_font.loadFromFile(fontPath))
        {
            std::cerr << "Failed to load font from: " << fontPath << std::endl;
        }

        m_pauseText.setFont(m_font);
        m_pauseText.setString("PAUSE");
        m_pauseText.setCharacterSize(60);
        m_pauseText.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = m_pauseText.getLocalBounds();
        m_pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        m_pauseText.setPosition(m_renderWindow->getSize().x / 2, m_renderWindow->getSize().y / 3);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading resources: " << e.what() << std::endl;
    }
}

void PauseMenu::initializeButtons()
{
    const float buttonWidth = 200.0f;
    const float buttonHeight = 60.0f;
    const float buttonSpacing = 30.0f;
    const float startY = m_renderWindow->getSize().y / 2;

    Button resumeButton
	(
        sf::Vector2f((m_renderWindow->getSize().x - buttonWidth) / 2, startY),
        sf::Vector2f(buttonWidth, buttonHeight),
        "RESUME",
        m_font,
        30
    );

    resumeButton.setFunction([this]() 
        {
        m_sceneManager->setCurrentScene("Game");
        SoundManager::getInstance().resumeMusic();
        });

    Button menuButton
	(
        sf::Vector2f((m_renderWindow->getSize().x - buttonWidth) / 2, startY + buttonHeight + buttonSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        "MENU",
        m_font,
        30
    );

    menuButton.setFunction([this]() 
        {
    	SoundManager::getInstance().stopMusic();

    	SoundManager::getInstance().playMusic("game_music", 20.0f);
        m_sceneManager->setCurrentScene("MainMenu");
        });

    m_buttons.push_back(resumeButton);
    m_buttons.push_back(menuButton);
}

void PauseMenu::processInput(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePosition = m_renderWindow->mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
        );

        for (auto& button : m_buttons)
        {
            button.isClicked(mousePosition);
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        m_sceneManager->setCurrentScene("Game");
    }

    SceneBase::processInput(event);
}

void PauseMenu::update(const float& deltaTime)
{
    sf::Vector2f mousePosition = m_renderWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_renderWindow));

    for (auto& button : m_buttons)
    {
        button.update(mousePosition);
    }

    SceneBase::update(deltaTime);
}

void PauseMenu::render()
{
    auto* gameScene = static_cast<SceneBase*>(m_sceneManager->getChild("Game"));
    if (gameScene)
    {
        gameScene->render();
    }

    m_renderWindow->draw(m_background);
    m_renderWindow->draw(m_pauseText);

    for (auto& button : m_buttons)
    {
        button.render(*m_renderWindow);
    }

    SceneBase::render();
}