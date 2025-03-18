#pragma once

#include "Tiles.h"

//class MapLayer
//{
//public:
//    MapLayer(int width = 20, int height = 20, float tileSize = 32.0f);
//    ~MapLayer() = default;
//
//    void initialize();
//    void render(sf::RenderWindow& window);
//
//    void setTile(int x, int y, int tileId, const std::string& tilesetName = "default_tileset");
//    TileComponent* getTile(int x, int y);
//
//    void clear();
//    void resize(int width, int height);
//
//    sf::Vector2i getSize() const;
//    float getTileSize() const;
//    void setTileSize(float size);
//
//    void setVisible(bool visible);
//    bool isVisible() const;
//    void setName(const std::string& name);
//    std::string getName() const;
//
//private:
//    std::vector<std::vector<std::shared_ptr<TileComponent>>> m_tiles;
//    int m_width;
//    int m_height;
//    float m_tileSize;
//    bool m_visible;
//    std::string m_name;
//
//    bool isValidPosition(int x, int y) const;
//};