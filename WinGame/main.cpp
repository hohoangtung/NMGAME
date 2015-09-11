#include <Windows.h>
#include <iostream>
#include "ContraGame.h"

using namespace std;
US_FRAMEWORK

#define APP_TITTLE L"Contra"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdLine,  int nCmdShow)
{
	Game* myGame;
	myGame = new ContraGame(hInstance, APP_TITTLE);

	myGame->init();
	myGame->run();
	myGame->release();

	delete myGame;
}