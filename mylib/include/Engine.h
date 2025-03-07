#pragma once
#include "SceneManager.h"

class Engine
{
public:
	Engine(SceneManager& scene);

	void exec();

private:
	SceneManager* m_sceneManager;
};