#include <iostream>
#include "Tiles.h"

TilesMap::TilesMap(const MapRepresentation& repr)
{
    m_width = static_cast<int>(repr[0].size());
    m_height = static_cast<int>(repr.size());
    m_tileSize = 16;

    m_tiles.resize(m_width * m_height);

    for (int row = 0; row < m_height; ++row)
    {
        for (int col = 0; col < m_width; ++col)
        {
            getTile(row, col) = repr[row][col];
        }
    }
}

sf::Vector2i TilesMap::getTileCoordinateInTexture(const TileType& tile)
{
    static std::map<TileType, sf::Vector2i> tileMap = {
         {'H', {4,  6} }
        ,{'C', {9,  2} }
    };

    auto it = tileMap.find(tile);
    if (it == tileMap.end())
        throw;

    return it->second;
}

int TilesMap::clamp(int min, int max, int val)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

void TilesMap::render(sf::RenderWindow& w)
{
    TextureManager& texManager = TextureManager::getInstance();

    if (!texManager.hasTexture("herbe2")) {
        texManager.loadTexture("herbe2", "Map\\herbe2.png");
    }

    sf::Texture* texture = texManager.getTexture("herbe2");
    if (!texture) {
        std::cerr << "Error: Map texture not found!" << std::endl;
        return;
    }

    sf::Sprite sprite;
    sprite.setTexture(*texture);

    sf::View view = w.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    int startRow = clamp(0, m_height - 1, static_cast<int>((viewCenter.y - viewSize.y / 2) / m_tileSize));
    int endRow = clamp(0, m_height - 1, static_cast<int>((viewCenter.y + viewSize.y / 2) / m_tileSize) + 1);
    int startCol = clamp(0, m_width - 1, static_cast<int>((viewCenter.x - viewSize.x / 2) / m_tileSize));
    int endCol = clamp(0, m_width - 1, static_cast<int>((viewCenter.x + viewSize.x / 2) / m_tileSize) + 1);

    for (int row = startRow; row <= endRow; ++row)
    {
        for (int col = startCol; col <= endCol; ++col)
        {
            try {
                TileType tileType = getTile(row, col);

                sf::Vector2i tileCoords = getTileCoordinateInTexture(tileType);

                sprite.setTextureRect(sf::IntRect(tileCoords.x * m_tileSize, tileCoords.y * m_tileSize,
                    m_tileSize, m_tileSize));

                sprite.setPosition(static_cast<float>(col * m_tileSize), static_cast<float>(row * m_tileSize));

                w.draw(sprite);
            }
            catch (const std::exception& e) {
                std::cerr << "Error rendering tile at (" << row << "," << col << "): " << e.what() << std::endl;
            }
        }
    }
}