#include "Engine.h"
#include "Game.h"

int main()
{
	Engine* engine(1920, 1080, "Test");
	Game game(engine);

	game.exec();

	return 0;
}