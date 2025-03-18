#pragma once

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Map.h"
#include "SceneBase.h"

class TilesMap
{
public:
    TilesMap(const MapRepresentation& repr);

    using TileType = char;

    sf::Vector2i getTileCoordinateInTexture(const TileType& tile);

    int clamp(int min, int max, int val);

    void render(sf::RenderWindow& w, const View& view);

private:
    TileType& getTile(int row, int col) { return m_tiles[row * m_width + col]; }
    const TileType& getTile(int row, int col) const { return m_tiles[row * m_width + col]; }

    std::vector<TileType> m_tiles;

    sf::Texture m_texture;

    int m_width;
    int m_height;
    int m_tileSize;
};

//class Tiles
//{
//public:
//    Tiles(const std::string& tilesetName = "");
//    ~Tiles() = default;
//
//    void initialize(int tileWidth = 32, int tileHeight = 32);
//
//    sf::IntRect getTileRect(int tileId) const;
//    std::string getTilesetName() const;
//    void setTilesetName(const std::string& name);
//
//    int getTileWidth() const;
//    int getTileHeight() const;
//
//private:
//    std::string m_tilesetName;
//    int m_tileWidth;
//    int m_tileHeight;
//    int m_columnsCount;
//    int m_rowsCount;
//};
//
//class TileComponent
//{
//public:
//    TileComponent(const std::string& tilesetName = " ");
//
//    void initialize();
//    void update(const float& deltaTime);
//    void render(sf::RenderWindow& window);
//
//    void setTileId(int id);
//    int getTileId() const;
//
//    void setPosition(int gridX, int gridY);
//    sf::Vector2i getGridPosition() const;
//    sf::Vector2f getWorldPosition() const;
//
//    void setSize(float size);
//    float getSize() const;
//
//    bool isWalkable() const;
//    void setWalkable(bool walkable);
//
//    void setTilesetName(const std::string& tilesetName);
//    std::string getTilesetName() const;
//
//private:
//    std::string m_tilesetName;
//    int m_tileId;
//    sf::Vector2i m_gridPosition;
//    float m_size;
//    sf::Sprite m_sprite;
//    bool m_walkable;
//    sf::IntRect m_textureRect;
//
//    void updateTextureRect();
//};
//
//class TileMap
//{
//public:
//    enum LayerType
//	{
//        BACKGROUND = 0,
//        MIDGROUND = 1,
//        FOREGROUND = 2,
//        COLLISION = 3,
//        LAYER_COUNT
//    };
//
//    TileMap(int width = 20, int height = 20, float tileSize = 32.0f);
//    ~TileMap() = default;
//
//    void initialize();
//    void update(const float& deltaTime);
//    void render(sf::RenderWindow& window);
//
//    MapLayer* getLayer(LayerType layer);
//    MapLayer* getLayer(const std::string& name);
//    MapLayer* getLayerAt(size_t index);
//    size_t getLayerCount() const;
//    void addLayer(const std::string& name);
//    void removeLayer(const std::string& name);
//    void moveLayerUp(const std::string& name);
//    void moveLayerDown(const std::string& name);
//
//    void setActiveLayer(LayerType layer);
//    void setActiveLayer(const std::string& name);
//    MapLayer* getActiveLayer();
//
//    void setTile(int x, int y, int tileId, const std::string& tilesetName = "default_tileset");
//    TileComponent* getTile(int x, int y);
//
//    void clear();
//    void resize(int width, int height);
//    sf::Vector2i getSize() const;
//    float getTileSize() const;
//    void setTileSize(float size);
//
//    bool loadFromFile(const std::string& filename);
//    bool saveToFile(const std::string& filename) const;
//
//    void setTiles(std::shared_ptr<Tiles> tiles);
//    Tiles* getTiles() const;
//
//private:
//    std::vector<std::shared_ptr<MapLayer>> m_layers;
//    LayerType m_activeLayerType;
//    size_t m_activeLayerIndex;
//    int m_width;
//    int m_height;
//    float m_tileSize;
//    std::shared_ptr<Tiles> m_tiles;
//
//    bool isValidLayerIndex(size_t index) const;
//};