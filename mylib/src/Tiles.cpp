#include <iostream>
#include "Tiles.h"

TilesMap::TilesMap(const MapRepresentation& repr)
{
    m_width = static_cast<int>(repr[0].size());
    m_height = static_cast<int>(repr.size());
    m_tileSize = 16;

    m_layers.resize(1);
    m_layers[0].tiles.resize(m_width * m_height);
    m_layers[0].tileSize = m_tileSize;

    for (int row = 0; row < m_height; ++row)
    {
        for (int col = 0; col < m_width; ++col)
        {
            getTile(0, row, col) = repr[row][col];
        }
    }
}

void TilesMap::addLayer(const MapRepresentation& repr, int tileSize)
{
    int layerWidth = static_cast<int>(repr[0].size());
    int layerHeight = static_cast<int>(repr.size());

    Layer newLayer;
    newLayer.tiles.resize(layerWidth * layerHeight);
    newLayer.tileSize = tileSize;

    for (int row = 0; row < layerHeight; ++row)
    {
        for (int col = 0; col < layerWidth; ++col)
        {
            newLayer.tiles[row * layerWidth + col] = repr[row][col];
        }
    }

    m_layers.push_back(newLayer);
}

void TilesMap::setLayerVisibility(int layerIndex, bool visible)
{
    if (layerIndex >= 0 && layerIndex < m_layers.size())
    {
        m_layers[layerIndex].visible = visible;
    }
}

void TilesMap::render(sf::RenderWindow& w)
{
    TextureManager& texManager = TextureManager::getInstance();

    if (!texManager.hasTexture("herbe2"))
    {
        texManager.loadTexture("herbe2", "Map\\herbe2.png");
    }

    if (!texManager.hasTexture("chemins"))
    {
        texManager.loadTexture("chemins", "Map\\chemins.png");
    }

    sf::View view = w.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    for (int layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex)
    {
        if (!m_layers[layerIndex].visible)
            continue;

        int layerTileSize = m_layers[layerIndex].tileSize;

        sf::Texture* texture = nullptr;
        if (layerTileSize == 16)
        {
            texture = texManager.getTexture("herbe2");
        }
        else if (layerTileSize == 32)
        {
            texture = texManager.getTexture("chemins");
        }

        if (!texture)
        {
            std::cerr << "Error: Texture not found for layer " << layerIndex << std::endl;
            continue;
        }

        sf::Sprite sprite;
        sprite.setTexture(*texture);

        int tileWidthRatio = layerTileSize / m_tileSize;
        int layerWidth = static_cast<int>(m_layers[layerIndex].tiles.size() /
            (m_layers[layerIndex].tiles.size() / m_width));
        int layerHeight = static_cast<int>(m_layers[layerIndex].tiles.size() / layerWidth);

        int startRow = clamp(0, layerHeight - 1, static_cast<int>((viewCenter.y - viewSize.y / 2) / layerTileSize));
        int endRow = clamp(0, layerHeight - 1, static_cast<int>((viewCenter.y + viewSize.y / 2) / layerTileSize) + 1);
        int startCol = clamp(0, layerWidth - 1, static_cast<int>((viewCenter.x - viewSize.x / 2) / layerTileSize));
        int endCol = clamp(0, layerWidth - 1, static_cast<int>((viewCenter.x + viewSize.x / 2) / layerTileSize) + 1);

        for (int row = startRow; row <= endRow; ++row)
        {
            for (int col = startCol; col <= endCol; ++col)
            {
                try
                {
                    if (row >= layerHeight || col >= layerWidth || row < 0 || col < 0)
                        continue;

                    TileType tileType = m_layers[layerIndex].tiles[row * layerWidth + col];

                    if (tileType == ' ' || tileType == '.')
                        continue;

                    sf::Vector2i tileCoords = getTileCoordinateInTexture(tileType);

                    sprite.setTextureRect(sf::IntRect(
                        tileCoords.x * layerTileSize,
                        tileCoords.y * layerTileSize,
                        layerTileSize,
                        layerTileSize
                    ));

                    float posX = static_cast<float>(col * layerTileSize);
                    float posY = static_cast<float>(row * layerTileSize);
                    sprite.setPosition(posX, posY);

                    w.draw(sprite);
                }
                catch (const std::exception& e) {
                    std::cerr << "Error rendering tile at layer " << layerIndex << " (" << row << "," << col << "): " << e.what() << std::endl;
                }
            }
        }
    }
}

sf::Vector2i TilesMap::getTileCoordinateInTexture(const TileType& tile)
{
    static std::map<TileType, sf::Vector2i> tileMap16 = {
        {'H', {4, 6}},
        {'C', {9, 2}},
        {'R', {9, 4}}
    };

    static std::map<TileType, sf::Vector2i> tileMap32 = {
        {'P', {1, 4}}
        /*,{'I', {1, 0}}
        ,{'T', {2, 0}}
        ,{'L', {3, 0}}
        ,{'V', {0, 1}}
        ,{'H', {1, 1}} */
    };

    auto it16 = tileMap16.find(tile);
    if (it16 != tileMap16.end())
        return it16->second;

    auto it32 = tileMap32.find(tile);
    if (it32 != tileMap32.end())
        return it32->second;

    throw std::runtime_error("Tile type not found: " + std::string(1, tile));
}

int TilesMap::clamp(int min, int max, int val)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

void TilesMap::setTile(int layerIndex, int row, int col, TileType tileType)
{
    if (layerIndex >= 0 && layerIndex < m_layers.size() &&
        row >= 0 && row < m_height &&
        col >= 0 && col < m_width)
    {
        getTile(layerIndex, row, col) = tileType;
    }
}