#include "TextureManager.h"
#include "PathManager.h"
#include <iostream>
#include <filesystem>

TextureManager::TextureManager()
    : m_initialized(false)
{
}

TextureManager::~TextureManager()
{
    cleanup();
}

void TextureManager::initialize()
{
    if (!m_initialized)
    {
        m_initialized = true;
        std::cout << "TextureManager initialized" << std::endl;
    }
}

void TextureManager::cleanup()
{
    m_textures.clear();
    m_initialized = false;
    std::cout << "TextureManager cleaned up" << std::endl;
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    if (hasTexture(name))
        return true;

    auto texture = std::make_unique<sf::Texture>();

    std::string fullPath = PathManager::getResourcePath(filename);

    if (!texture->loadFromFile(fullPath))
    {
        std::cerr << "Failed to load texture: " << fullPath << std::endl;
        return false;
    }

    texture->setSmooth(true);

    m_textures[name] = std::move(texture);
    std::cout << "Loaded texture: " << name << " from " << fullPath << std::endl;

    return true;
}

sf::Texture* TextureManager::getTexture(const std::string& name)
{
    auto iterator = m_textures.find(name);
    if (iterator != m_textures.end())
    {
        return iterator->second.get();
    }

    std::cerr << "Texture not found: " << name << std::endl;
    return nullptr;
}

bool TextureManager::hasTexture(const std::string& name) const
{
    return m_textures.find(name) != m_textures.end();
}

void TextureManager::loadAllGameTextures()
{
    if (!m_initialized)
        initialize();

    loadTexture("player", "hero\\player.png");
    loadTexture("enemy", "enemy\\enemy.png");
    loadTexture("arrow", "projectile\\arrow.png");
    loadTexture("blood", "effect\\blood.png");
    loadTexture("hero_icons", "hud\\hero_icons.png");
    loadTexture("health_bar_template", "hud\\health_bar_template.png");

    std::cout << "All game textures loaded successfully\n";
}