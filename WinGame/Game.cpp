#include <string>
#include "Game.h"
#include "FrameWork\Event.h"

using namespace std;
US_FRAMEWORK

#if _DEBUG
#include "debug.h"	// for print to output. call: __debugoutput()
#endif // _DEBUG

int Game::isExit = 0;
Graphics* Game::hWindow = NULL;
Graphics* Game::getWindow()
{
	return hWindow;
}
void Game::exit()
{
	isExit = 1;
}

Game::~Game(void)
{
	// Do nothing. Use release instead
}


Game::Game(HINSTANCE hInstance, LPWSTR name, int width, int height, int fps, int isFullScreen)
{
	//this->wnd_Instance = new Graphics(hInstance, name, width, height, fps, isFullScreen);
	hWindow = new Graphics(hInstance, name, width, height, fps, isFullScreen);
	_gametime = GameTime::getInstance();
	_devicemanager = DeviceManager::getInstance();
	_input = InputController::getInstance();
	_spriteHandle = NULL;
}

void Game::init()
{
	//wnd_Instance->initWindow();
	if (hWindow == NULL)
		throw;
	hWindow->initWindow();
	_gametime->init();
	_devicemanager->Init(*hWindow);
	//_devicemanager->Init(*wnd_Instance);
	_input->init(hWindow->getWnd(), hWindow->gethInstance());
	//_input->init(wnd_Instance->getWnd(), wnd_Instance->gethInstance());
	this->_frameRate = 1000.0f / hWindow->getFrameRate();	 //1000/30 = 33 milisecond

	D3DXCreateSprite(_devicemanager->getDevice(), &this->_spriteHandle);
	this->loadResource();

	_oldTime = _gametime->getTotalGameTime();
	_deltaTime = 0.0f;
}

void Game::run()
{
	MSG msg;
	while (isExit == 0)
	{
#pragma region Translate Message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				isExit = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}										// dont mention it.  see ebook if you want more info
#pragma endregion

		_gametime->updateGameTime();							// gametime isn't run if dont call updateGameTime
		_deltaTime = _gametime->getTotalGameTime() - _oldTime;

		if (_deltaTime >= _frameRate)
		{
			_oldTime += _frameRate;
			_input->update();
			this->render();
		}
		else
			Sleep(_frameRate - _deltaTime);						//sleep every frame for high performance
	}
}
void Game::render()												// call once per frame
{
	// kiểm tra nếu cửa sổ đang focus không phải game thì không cập nhật
	if (GetActiveWindow() != hWindow->getWnd())
		return;
	auto device = _devicemanager->getInstance();
	float time = _gametime->getElapsedGameTime();

	// để xử lý kéo cửa sổ không vị dồn frame
	// vì chỉ là thủ thuật set cứng thời gian
	// nên bất kỳ đối tượng nào không update theo thời gian thì khi kéo cửa sổ sẽ bị dồn frame
	if (time > this->_frameRate * 2)
	{
		time = _frameRate;
	}

	if (device->getDevice()->BeginScene() != DI_OK)
		return;
	device->clearScreen();
	// main game's logic
	updateInput(time);
	update(time);
	draw();

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
	//SAFE_DELETE(_devicemanager);
	_gametime->release();
	//SAFE_DELETE(_gametime);
	//_input->release();
	//SAFE_DELETE(_input);
}

