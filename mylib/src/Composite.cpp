#include "Composite.h"

CompositeGameObject::CompositeGameObject(const std::string& name)
	: GameObject(name)
{
}

void CompositeGameObject::processInput(const sf::Event& event)
{
	for (auto& component : m_components)
		component.second->processInput(event);

	GameObject::processInput(event);
}

void CompositeGameObject::update(const float& deltaTime)
{
	for (auto& component : m_components)
		component.second->update(deltaTime);

	GameObject::update(deltaTime);
}

void CompositeGameObject::render(sf::RenderWindow& window)
{
	for (auto& component : m_components)
		component.second->render(window);

	GameObject::render(window);
}

void CompositeGameObject::addComponent(std::shared_ptr<Component> component)
{
	component->setOwner(this);
	component->initialize();
	m_components[component->getName()] = component;
}

Component* CompositeGameObject::getComponent(const std::string& name)
{
	auto iterator = m_components.find(name);
	if (iterator != m_components.end())
		return iterator->second.get();

	return nullptr;
}

const Component* CompositeGameObject::getComponent(const std::string& name) const
{
	auto iterator = m_components.find(name);
	if (iterator != m_components.end())
		return iterator->second.get();

	return nullptr;
}

void CompositeGameObject::removeComponent(const std::string& name)
{
	auto iterator = m_components.find(name);
	if (iterator != m_components.end())
		m_components.erase(iterator);
}


// SCENE COMPOSITE
CompositeScene::CompositeScene(sf::RenderWindow* window, const float& framerate, const std::string& name)
	: SceneBase(window, framerate, name)
{
}

void CompositeScene::processInput(const sf::Event& event)
{
	for (auto& child : m_children)
		child->processInput(event);
}

void CompositeScene::update(const float& deltaTime)
{
	for (auto& child : m_children)
		child->update(deltaTime);
}

void CompositeScene::render()
{
	for (auto& child : m_children)
		child->render();
}