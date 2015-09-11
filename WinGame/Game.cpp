#include "Game.h"
#include "FrameWork\Event.h"
#include <string>
using namespace std;
US_FRAMEWORK

#if _DEBUG
#include "debug.h"	// for print to output. call: __debugoutput()
#endif // _DEBUG

int Game::isExit = 0;

void Game::exit()
{
	isExit = 1;
}
// Tạm để đây để test sprite. Có thể ssau này bỏ vô class 
//static LPD3DXSPRITE g_spritehandle = nullptr;
Sprite *p;	// for test

Game::~Game(void)
{
	// Do nothing. Use release instead
}


Game::Game(HINSTANCE hInstance, LPWSTR name, int width, int height, int fps, int isFullScreen)
{
	this->wnd_Instance = new Graphics(hInstance, name, width, height, fps, isFullScreen);
	_gametime = GameTime::getInstance();
	_devicemanager = DeviceManager::getInstance();
	_input = InputController::getInstance();
	_spriteHandle = NULL;
}

void Game::init()
{
	wnd_Instance->initWindow();
	_gametime->init();
	_devicemanager->Init(*wnd_Instance);
	_input->init(wnd_Instance->getWnd(), wnd_Instance->gethInstance());
	this->_frameRate = 1000.0f / wnd_Instance->getFrameRate();	 //1000/30 = 33 milisecond

	D3DXCreateSprite(_devicemanager->getDevice(), &this->_spriteHandle);
	p = new Sprite(this->_spriteHandle,L"Flower.png",4, 4);

	_oldTime = _gametime->getTotalGameTime();
	_deltaTime = 0.0f;

	// load resource
	this->loadResource();

}

static StopWatch *sw = new StopWatch();	// test
void Game::run()
{
	MSG msg;
	while (isExit == 0)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				isExit = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		// dont mention it.  see ebook if you want more info

		_gametime->updateGameTime();				// gametime isn't run if dont call updateGameTime
		_deltaTime = _gametime->getTotalGameTime() - _oldTime;
		if (_deltaTime > _frameRate)
		{
			_oldTime += _frameRate;
			_input->update();
			this->render();
		}
		else
			Sleep(_frameRate - _deltaTime);			//sleep every frame for high performance
	}
}
void Game::render()		// call once per frame
{
	// should go to another place
	auto device = _devicemanager->getInstance();
	float time = _gametime->getElapsedGameTime();
	if (device->getDevice()->BeginScene() != DI_OK)
		return;
	device->clearScreen();
	{
		// main game's logic
		updateInput(time);
		update(time);
		draw();
	}
	device->getDevice()->EndScene();
	device->present();

}

void Game::draw()
{
}

void Game::updateInput(float deltatime)
{
	// do nothing.
	// override this for effection
}
void Game::update(float deltatime)
{
	// do nothing.
	// override this for effection
}
void Game::loadResource()
{
	// do nothing.
	// override this for effection
}
void Game::release()
{
	_devicemanager->release();
	_gametime->release();
	_input->release();
}

