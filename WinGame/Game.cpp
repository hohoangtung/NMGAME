#include <string>
#include "Game.h"
#include "FrameWork\Event.h"

#if _DEBUG
#include "debug.h"
#endif // _DEBUG

using namespace std;
US_FRAMEWORK

StopWatch* Game::g_StopWatch = new StopWatch();
int Game::isExit = 0;

void Game::exit()
{
	isExit = 1;
}

// Tạm để đây để test sprite. Có thể sau này bỏ vô class 
static LPD3DXSPRITE g_spritehandle = nullptr;
Sprite *p;

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
}

void A(KeyEventArg *e)
{
	if (e->_key == DIK_SPACE)
	{
		OutputDebugString(L"down\n");
	}
}

void B(KeyEventArg *e)
{
	if (e->_key == DIK_SPACE)
	{
		OutputDebugString(L"up\n");
	}
}

void Game::init()
{
	wnd_Instance->initWindow();
	_gametime->init();
	_devicemanager->Init(*wnd_Instance);
	_input->init(wnd_Instance->getWnd(), wnd_Instance->gethInstance());
	this->_frameRate = 1000.0f / wnd_Instance->getFrameRate(); //1000/30 = 33 milisecond

	D3DXCreateSprite(_devicemanager->getDevice(), &g_spritehandle);
	p = new Sprite(g_spritehandle,L"Flower.png",4, 4);

	_input->_keyPressed += (EventFunction)&A;		// test
	_input->_keyReleased += (EventFunction)&B;		// test
	_oldTime = _gametime->getTotalGameTime();
	_deltaTime = 0.0f;
}

void Action(void)
{
	OutputDebugString(L"Action\n");
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
		}
		_gametime->updateGameTime();
		_deltaTime = _gametime->getTotalGameTime() - _oldTime;
		if (_deltaTime > _frameRate)
		{
			_oldTime += _frameRate;
			_input->update();
			this->render();
		}
		else
			Sleep(_frameRate - _deltaTime);
	}
}

void Game::render()
{
	// should go to another place
	auto device = _devicemanager->getInstance();
	float time = _gametime->getElapsedGameTime();
	if (device->getDevice()->BeginScene() != DI_OK)
		return;
	device->clearScreen();
	{
		updateInput(time);
		draw(time);
	}
	device->getDevice()->EndScene();
	device->present();

}

void Game::draw(float deltatime)
{
	// should go to another classs
	g_spritehandle->Begin(D3DXSPRITE_ALPHABLEND);
	p->render(g_spritehandle);
	//g_spritehandle->Flush();
	p->next();
	g_spritehandle->End();
	// ----

}

void Game::updateInput(float deltatime)
{
	// do something
}

void Game::release()
{
	_devicemanager->release();
	_gametime->release();
	_input->release();
}

