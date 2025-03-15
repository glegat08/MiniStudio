#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class CompositeGameObject;
class CameraTargetAdapter;

class Component
{
public:
	Component(const std::string& name = "Component");
	virtual ~Component() = default;

	virtual void initialize();
	virtual void processInput(const sf::Event& event);
	virtual void update(const float& deltaTime);
	virtual void render(sf::RenderWindow& window);

	void setOwner(CompositeGameObject* owner);
	CompositeGameObject* getOwner() const;

	std::string getName() const;
	void setName(const std::string& name);

protected:
	CompositeGameObject* m_owner;
	std::string m_name;
};

class SquareRenderer : public Component
{
public:
	SquareRenderer(float size = 50.0f, const sf::Color& color = sf::Color::Red);
	~SquareRenderer() = default;

	void initialize() override;
	void render(sf::RenderWindow& window) override;

	void setSize(float size);
	void setColor(const sf::Color& color);
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;

private:
	sf::RectangleShape m_shape;
	float m_size;
	sf::Color m_color;
};

class PlayerController : public Component
{
public:
	PlayerController(float speed = 200.0f);
	~PlayerController() = default;

	void initialize() override;
	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;

	void setSpeed(float speed);
	float getSpeed() const;

	bool isMoving() const;
	bool isMovingUp() const;
	bool isMovingDown() const;
	bool isMovingLeft() const;
	bool isMovingRight() const;

	friend class CameraTargetAdapter;

private:
	float m_speed;
	sf::Vector2f m_direction;
	sf::Vector2f m_velocity;
	bool m_isMovingUp;
	bool m_isMovingDown;
	bool m_isMovingLeft;
	bool m_isMovingRight;
};

class TileComponent : public Component
{
public:
    TileComponent(const std::string& tilesetName = " ");

    void initialize() override;
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setTileId(int id);
    int getTileId() const;

    void setPosition(int gridX, int gridY);
    sf::Vector2i getGridPosition() const;
    sf::Vector2f getWorldPosition() const;

    void setSize(float size);
    float getSize() const;

    bool isWalkable() const;
    void setWalkable(bool walkable);

    void setTilesetName(const std::string& tilesetName);
    std::string getTilesetName() const;

private:
    std::string m_tilesetName;
    int m_tileId;
    sf::Vector2i m_gridPosition;
    float m_size;
    sf::Sprite m_sprite;
    bool m_walkable;
    sf::IntRect m_textureRect;

    void updateTextureRect();
};

// Class TileMap to regroup the tiles
class TileMap : public Component
{
public:
    TileMap(int width = 20, int height = 20, float tileSize = 32.0f);

    void initialize() override;
    void update(const float& deltaTime) override;
    void render(sf::RenderWindow& window) override;

    void setTile(int x, int y, int tileId, const std::string& tilesetName = "default_tileset");
    TileComponent* getTile(int x, int y);

    void clear();
    void resize(int width, int height);

    sf::Vector2i getSize() const;
    float getTileSize() const;
    void setTileSize(float size);

    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;

private:
    std::vector<std::vector<std::shared_ptr<TileComponent>>> m_tiles;
    int m_width;
    int m_height;
    float m_tileSize;

    bool isValidPosition(int x, int y) const;
};