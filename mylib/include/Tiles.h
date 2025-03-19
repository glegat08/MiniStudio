#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "PathManager.h"
#include "TextureManager.h"

using MapRepresentation = std::vector<std::string>;
extern MapRepresentation repr;

class TilesMap
{
public:
    TilesMap(const MapRepresentation& repr);

    using TileType = char;

    sf::Vector2i getTileCoordinateInTexture(const TileType& tile);

    int clamp(int min, int max, int val);

    void render(sf::RenderWindow& w);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    TileType& getTile(int row, int col) { return m_tiles[row * m_width + col]; }
    const TileType& getTile(int row, int col) const { return m_tiles[row * m_width + col]; }

    sf::Texture m_texture;

    int m_width = 32;
    int m_height = 32;
    int m_tileSize;
    std::vector<TileType> m_tiles;
};