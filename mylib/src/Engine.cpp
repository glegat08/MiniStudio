#include "Engine.h"

Engine::Engine(SceneManager& scene)
	: m_sceneManager(&scene)
{

}


void Engine::exec()
{
	m_sceneManager->exec();
}