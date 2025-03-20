#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

#include "PathManager.h"
#include "TextureManager.h"

using MapRepresentation = std::vector<std::string>;
extern MapRepresentation repr;

using LayerRepresentation = std::vector<std::string>;
extern LayerRepresentation path;

class TilesMap
{
public:
    TilesMap(const MapRepresentation& repr);

    using TileType = char;

    struct Layer
	{
        std::vector<TileType> tiles;
        bool visible = true;
        int tileSize = 16;
    };

    void addLayer(const MapRepresentation& repr, int tileSize = 16);
	void setLayerVisibility(int layerIndex, bool visible);
    int getLayerCount() const { return static_cast<int>(m_layers.size()); }

    void setTile(int layerIndex, int row, int col, TileType tileType);

    void render(sf::RenderWindow& w);

    sf::Vector2i getTileCoordinateInTexture(const TileType& tile);

    int clamp(int min, int max, int val);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    TileType& getTile(int layerIndex, int row, int col) {return m_layers[layerIndex].tiles[row * m_width + col];}

    const TileType& getTile(int layerIndex, int row, int col) const {return m_layers[layerIndex].tiles[row * m_width + col];}

    int m_width;
    int m_height;
    int m_tileSize;
    std::vector<TileType> m_tiles;
    std::vector<Layer> m_layers;
};