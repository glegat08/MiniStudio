#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class CompositeGameObject;
class TileMap;

class SceneBase
{
public:
    SceneBase(sf::RenderWindow* window, const float& framerate, const std::string& name = "Scene");
    virtual ~SceneBase() = default;

    virtual void processInput(const sf::Event& event);
    virtual void update(const float& deltaTime);
    virtual void render();

    virtual void addChild(std::unique_ptr<SceneBase> child);
    virtual void removeChild(const std::string& name);
    virtual SceneBase* getChild(const std::string& name);

    sf::Time getRefreshTime() const;
    std::string getName() const;
    void setName(const std::string& name);
    static SceneBase* getCurrentScene();

protected:
    sf::RenderWindow* m_renderWindow;
    sf::Time m_refreshTime;
    std::string m_name;
    std::vector<std::unique_ptr<SceneBase>> m_children;

private:
    static SceneBase* m_currentScene;
};

class MapEditor : public SceneBase
{
public:
    MapEditor(sf::RenderWindow* window, const float& framerate = 60.0f, const std::string& name = "MapEditor");

    void initialize();
    void processInput(const sf::Event& event) override;
    void update(const float& deltaTime) override;
    void render() override;

    void newMap(int width, int height, float tileSize = 32.0f);
    bool loadMap(const std::string& filename);

    void setCurrentTile(int tileId, const std::string& tilesetName = "");
    void addTileset(const std::string& name, const std::string& filename);

private:
    std::shared_ptr<CompositeGameObject> m_mapObject;
    std::shared_ptr<TileMap> m_tileMap;

    int m_currentTileId;
    std::string m_currentTilesetName;

    std::unordered_map<std::string, std::string> m_tilesets;

    bool m_gridVisible;
    bool m_showCollisions;
	
    void drawGrid();
};