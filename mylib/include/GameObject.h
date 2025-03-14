#pragma once
#include "IGameObject.h"
#include <algorithm>
#include <unordered_set>

class GameObject : public IGameObject
{
public:
	GameObject(const std::string& name = "GameObject");
	virtual ~GameObject() = default;

	void processInput(const sf::Event& event) override;
	void update(const float& deltaTime) override;
	void render(sf::RenderWindow& window) override;

	void addChild(std::shared_ptr<IGameObject> child) override;
	void removeChild(const std::string& name) override;
	IGameObject* getChild(const std::string& name) override;

	void setName(const std::string& name) override;
	std::string getName() const override;

	void setCategory(const std::string& category);
	std::string getCategory() const;

	void addTag(const std::string& tag);
	void removeTag(const std::string& tag);
	bool hasTag(const std::string& tag) const;
	const std::unordered_set<std::string>& getTags() const;

protected:
	std::string m_name;
	std::string m_category;
	std::unordered_set<std::string> m_tags;
	std::vector<std::shared_ptr<IGameObject>> m_children;
};
