#include "Tiles.h"

MapRepresentation repr = {
    "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
  , "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH"
};

void View::update()
{
    float factor = 0.01f;
    posX = (1.f - factor) * posX + factor * targetX;
    posY = (1.f - factor) * posY + factor * targetY;
}

TilesMap::TilesMap(const MapRepresentation& repr)
{
    m_width = static_cast<int>(repr[0].size());
    m_height = static_cast<int>(repr.size());
    m_tileSize = 32;

    m_tiles.resize(m_width* m_height);

    for (int row = 0; row < m_height; ++row)
    {
        for (int col = 0; col < m_width; ++col)
        {
            getTile(row, col) = repr[row][col];
        }
    }
    m_texture.loadFromFile("map\\herbe.png");
}

sf::Vector2i TilesMap::getTileCoordinateInTexture(const TileType& tile)
{
    static std::map<TileType, sf::Vector2i> tileMap = {
        {'H', {5,  2} }
      //, {'F', {0,  3} }
      //, {'T', {12, 6} }
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

void TilesMap::render(sf::RenderWindow& w, const View& view)
{
    float tileSizeInScreen = m_tileSize * view.scale;

    int beginTileX = static_cast<int>(view.posX / tileSizeInScreen);
    int beginTileY = static_cast<int>(view.posY / tileSizeInScreen);

    int nbTileX = static_cast<int>(std::ceil(static_cast<float>(view.width) / tileSizeInScreen));
    int nbTileY = static_cast<int>(std::ceil(static_cast<float>(view.height) / tileSizeInScreen));

    int endTileX = beginTileX + nbTileX;
    int endTileY = beginTileY + nbTileY;

    beginTileX = clamp(0, m_width - 1, beginTileX);
    endTileX = clamp(0, m_width - 1, endTileX);

    beginTileY = clamp(0, m_height - 1, beginTileY);
    endTileY = clamp(0, m_height - 1, endTileY);


    for (int row = beginTileY; row <= endTileY; ++row)
    {
        for (int col = beginTileX; col <= endTileX; ++col)
        {
            sf::Vector2f screenPos = sf::Vector2f(tileSizeInScreen * col - view.posX, tileSizeInScreen * row - view.posY);

            sf::Vector2i tilePosInTex = m_tileSize * getTileCoordinateInTexture(getTile(row, col));
            sf::Sprite sprite(m_texture, sf::IntRect{ tilePosInTex, {m_tileSize, m_tileSize} });
            sprite.setScale(view.scale, view.scale);
            sprite.setPosition(screenPos);
            w.draw(sprite);
        }
    }
}

//Tiles::Tiles(const std::string& tilesetName)
//    : m_tilesetName(tilesetName)
//    , m_tileWidth(32)
//    , m_tileHeight(32)
//    , m_columnsCount(0)
//    , m_rowsCount(0)
//{
//}
//
//void Tiles::initialize(int tileWidth, int tileHeight)
//{
//    m_tileWidth = tileWidth;
//    m_tileHeight = tileHeight;
//
//    sf::Texture* texture = TextureManager::getInstance().getTexture(m_tilesetName);
//    if (texture)
//    {
//        sf::Vector2u texSize = texture->getSize();
//        m_columnsCount = texSize.x / m_tileWidth;
//        m_rowsCount = texSize.y / m_tileHeight;
//    }
//}
//
//sf::IntRect Tiles::getTileRect(int tileId) const
//{
//    if (tileId <= 0 || m_columnsCount == 0)
//        return sf::IntRect(0, 0, 0, 0);
//
//    int tileX = (tileId - 1) % m_columnsCount;
//    int tileY = (tileId - 1) / m_columnsCount;
//
//    return sf::IntRect(tileX * m_tileWidth, tileY * m_tileHeight, m_tileWidth, m_tileHeight);
//}
//
//std::string Tiles::getTilesetName() const
//{
//    return m_tilesetName;
//}
//
//void Tiles::setTilesetName(const std::string& name)
//{
//    m_tilesetName = name;
//    initialize(m_tileWidth, m_tileHeight);
//}
//
//int Tiles::getTileWidth() const
//{
//    return m_tileWidth;
//}
//
//int Tiles::getTileHeight() const
//{
//    return m_tileHeight;
//}