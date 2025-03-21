#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class TextureManager
{
public:
    static TextureManager& getInstance()
    {
        static TextureManager instance;
        return instance;
    }

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    void initialize();
    void cleanup();

    bool loadTexture(const std::string& name, const std::string& filename);

    sf::Texture* getTexture(const std::string& name);

    void removeTexture(const std::string& name);

    bool hasTexture(const std::string& name) const;

    sf::Vector2u getTextureDimensions(const std::string& name) const;

    bool loadTexturesFromDirectory(const std::string& directory, const std::string& pattern = "*");

    std::vector<std::string> getLoadedTextureNames() const;

    void loadAllGameTextures();

private:
    TextureManager();
    ~TextureManager();

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    bool m_initialized;
};