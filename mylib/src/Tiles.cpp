#include <iostream>
#include "Tiles.h"

TilesMap::TilesMap(const std::string& name, const MapRepresentation& repr, int tileSize)
    : CompositeGameObject(name)
{
    m_width = static_cast<int>(repr[0].size());
    m_height = static_cast<int>(repr.size());
    m_tileSize = tileSize;

    m_layers.resize(1);
    m_layers[0].tiles.resize(m_width * m_height);
    m_layers[0].tileSize = tileSize;

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
        float scaleFactor = (layerTileSize == 32) ? m_scale.x : 1.0f;
        float scaledTileSize = layerTileSize * scaleFactor;

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

        if (layerTileSize == 32)
            sprite.setScale(m_scale);

        int layerWidth = m_width;
        int layerHeight = static_cast<int>(m_layers[layerIndex].tiles.size() / layerWidth);

        int startRow = clamp(0, layerHeight - 1, static_cast<int>((viewCenter.y - viewSize.y / 2) / scaledTileSize));
        int endRow = clamp(0, layerHeight - 1, static_cast<int>((viewCenter.y + viewSize.y / 2) / scaledTileSize) + 1);
        int startCol = clamp(0, layerWidth - 1, static_cast<int>((viewCenter.x - viewSize.x / 2) / scaledTileSize));
        int endCol = clamp(0, layerWidth - 1, static_cast<int>((viewCenter.x + viewSize.x / 2) / scaledTileSize) + 1);

        startRow = std::max(0, startRow - 1);
        endRow = std::min(layerHeight - 1, endRow + 1);
        startCol = std::max(0, startCol - 1);
        endCol = std::min(layerWidth - 1, endCol + 1);

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

                    if (layerTileSize == 32)
                    {
                        float posX = static_cast<float>(col * layerTileSize * m_scale.x);
                        float posY = static_cast<float>(row * layerTileSize * m_scale.y);
                        sprite.setPosition(posX, posY);
                    }
                    else
                    {
                        float posX = static_cast<float>(col * layerTileSize);
                        float posY = static_cast<float>(row * layerTileSize);
                        sprite.setPosition(posX, posY);
                    }
                    w.draw(sprite);
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Error rendering tile at layer " << layerIndex << " (" << row << "," << col << "): " << e.what() << std::endl;
                }
            }
        }
    }
}
sf::Vector2i TilesMap::getTileCoordinateInTexture(const TileType& tile)
{
    static std::map<TileType, sf::Vector2i> tileMap16 = {
         {'H', {4, 6}}
        ,{'C', {9, 2}}
        ,{'R', {9, 4}}
    };

    static std::map<TileType, sf::Vector2i> tileMap32 = {
         {'[', {0, 3}}
        ,{']', {2, 3}}
        ,{'S', {3, 0}}
        ,{'F', {3, 2}}
        ,{'|', {3, 1}}
        ,{'-', {1, 3}} 
        ,{'+', {3, 3}} 
		,{'7', {0, 0}}
		,{'8', {1, 0}}
		,{'9', {2, 0}}
		,{'4', {0, 1}}
		,{'5', {1, 1}}
		,{'6', {2, 1}}
		,{'1', {0, 2}}
		,{'2', {1, 2}}
		,{'3', {2, 2}}
    };

    auto it16 = tileMap16.find(tile);
    if (it16 != tileMap16.end())
        return it16->second;

    auto it32 = tileMap32.find(tile);
    if (it32 != tileMap32.end())
        return it32->second;

    throw std::runtime_error("Tile type not found: " + std::string(1, tile));
}

void TilesMap::setScale(float scaleX, float scaleY)
{
    m_scale.x = scaleX;
    m_scale.y = scaleY;
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