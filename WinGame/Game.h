
/*
	HOW TO USE:

	this is a basic game class.
	inheritance it to which game you want to build
	override virtual methods (init, release, draw, updateinput, update)
	
	example:
	class GameX: public Game
	{
	public:
		void init() 
		{
			Game::init();
			some objects created
		}				
		void release()
		{
			Game::release();
			some objects deleted
		}

		void updateInput(float deltatime) {check input event and implement action}
		void update(float deltatime){ update all object on screen}
		void draw(float deltatime) {draw to screen}

		//5 overrided methods

		// updateinput, update, draw. they are called automatic in Game::run()
	}

	in main function:
	call:
	game* game = new GameX(...);
	game->init();
	game->run();
	game->release();
	delete game;

	when you build GameY. just replace "new GameX(...)" into "new GameY(...)"
*/
#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>
#include "FrameWork\Graphics.h"
#include "FrameWork\DeviceManager.h"
#include "FrameWork\Sprite.h"
#include "FrameWork\GameTime.h"
#include "FrameWork\StopWatch.h"
#include "FrameWork\InputController.h"
#include "FrameWork\Managers\SceneManager.h"
#include "objects\PlayScene.h"

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

//define basic game's loop
class Game
{
public:
	virtual ~Game(void);
	
	static int isExit;

	Game(HINSTANCE, LPWSTR = L"Window Game", int width = 800, int height = 600, int fps = 30, int isFullScreen = 0);
	void virtual init();						// init your objects
	void virtual release();						// release your objects
	
	void virtual updateInput(float deltatime);	// update input, include played objects and menu button
	void virtual update(float deltatime);		// update your objects
	void virtual draw();						// draw your objects
	void virtual loadResource();

	void run();
	void render();		
	static void exit();			// exit the game. call: Game::exit()
	static Graphics* getWindow();
protected:
	pGraphics wnd_Instance;		// for init window. 
	pGameTime _gametime;		
	pDeviceManager _devicemanager;
	pInputController _input;
	LPD3DXSPRITE _spriteHandle;
	static Graphics *hWindow;
private:
	float _frameRate;			// time for 1 frame, milisecond
	float _oldTime;
	float _deltaTime;
};

typedef Game* pGame;

NS_FRAMEWORK_END

#endif // !__GAME_H__

