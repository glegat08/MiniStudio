#include <iostream>

#include "Engine.h"
#include "PathManager.h"

int main(int argc, char* argv[])
{
	PathManager::initialize(argv[0]);
	SceneManager engine(1920, 1080, "Test");
	Engine game(engine);
	
	game.exec();

	return 0;
}
