#pragma once
#include <unordered_map>

#include "Component.h"
#include "GameObject.h"
#include "SceneBase.h"

class CompositeGameObject : public GameObject
{
public:
	CompositeGameObject(const std::string& name = "CompositeGameObject");
	virtual ~CompositeGameObject() = default;

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render(sf::RenderWindow& window) override;

	void addComponent(std::shared_ptr<Component> component);
	Component* getComponent(const std::string& name);
	const Component* getComponent(const std::string& name) const;
	void removeComponent(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> m_components;
};

class CompositeScene : public SceneBase
{
public:
	CompositeScene(sf::RenderWindow* window, const float& framerate, const std::string& name = "CompositeScene");
	~CompositeScene() = default;

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render() override;
};