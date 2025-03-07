#include "Engine.h"

int main(int argc, char* argv[])
{
	SceneManager engine(1920, 1080, "Test");
	Engine game(engine);

	game.exec();

	return 0;
}