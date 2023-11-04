#include "Game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	//int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	//int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth = 800;
	int screenHeight = 600;

	Game *game = new Game(screenWidth, screenHeight);

	game->run();

	delete game;

	return 0;
}