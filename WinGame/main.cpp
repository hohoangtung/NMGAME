#include <Windows.h>
#include "Game.h"
#include <iostream>
using namespace std;
US_FRAMEWORK

#define APP_TITTLE L"Game Title"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine,  int nCmdShow)
{
	Game* myGame;
	myGame = new Game(hInstance, APP_TITTLE);

	myGame->init();
	myGame->run();
	myGame->release();
	delete myGame;
}