

#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include "FrameWork\Graphics.h"
#include "FrameWork\DeviceManager.h"
#include "FrameWork\Sprite.h"
#include "FrameWork\GameTime.h"
#include "FrameWork\StopWatch.h"
#include "FrameWork\InputController.h"

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

class Game
{
public:
	~Game(void);
	static StopWatch *g_StopWatch;
	static int isExit;

	Game(HINSTANCE, LPWSTR = L"Window Game", int width = 800, int height = 600, int fps = 30, int isFullScreen = 0);
	void virtual init();
	void virtual run();
	void virtual release();
	void virtual draw(float deltatime);			// để tạm
	void virtual updateInput(float deltatime);
	void render();		
	static void exit();			// could call every where to exit the game.
protected:
	pGraphics wnd_Instance;
	pGameTime _gametime;
	pDeviceManager _devicemanager;
	pInputController _input;
	float _frameRate;			// time for 1 frame, milisecond
	float _oldTime;
	float _deltaTime;

};

typedef Game* pGame;

NS_FRAMEWORK_END

#endif // !__GAME_H__

