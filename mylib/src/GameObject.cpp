#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	: m_name(name)
{
}

void GameObject::processInput(const sf::Event& event)
{
	for (auto& child : m_children)
		child->processInput(event);
}

void GameObject::update(const float& deltaTime)
{
	for (auto& child : m_children)
		child->update(deltaTime);
}

void GameObject::render(sf::RenderWindow& window)
{
	for (auto& child : m_children)
		child->render(window);
}

void GameObject::addChild(std::shared_ptr<IGameObject> child)
{
	m_children.push_back(child);
}

void GameObject::removeChild(const std::string& name)
{
	auto it = std::find_if(m_children.begin(), m_children.end(),
		[&name](const std::shared_ptr<IGameObject>& obj) 
		{
			return obj->getName() == name;
		});

	if (it != m_children.end())
		m_children.erase(it);
}

IGameObject* GameObject::getChild(const std::string& name)
{
	for (auto& child : m_children)
	{
		if (child->getName() == name)
			return child.get();
	}

	return nullptr;
}

void GameObject::setName(const std::string& name)
{
	m_name = name;
}

std::string GameObject::getName() const
{
	return m_name;
}

void GameObject::setCategory(const std::string& category)
{
	m_category = category;
}

std::string GameObject::getCategory() const
{
	return m_category;
}

void GameObject::addTag(const std::string& tag)
{
	m_tags.insert(tag);
}

void GameObject::removeTag(const std::string& tag)
{
	m_tags.erase(tag);
}

bool GameObject::hasTag(const std::string& tag) const
{
	return m_tags.find(tag) != m_tags.end();
}

const std::unordered_set<std::string>& GameObject::getTags() const
{
	return m_tags;
}