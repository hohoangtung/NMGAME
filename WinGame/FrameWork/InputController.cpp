#include "InputController.h"

US_FRAMEWORK

InputController::InputController()
{
	_input = NULL;
	_keyboard = NULL;
	ZeroMemory(_keyBuffer, 256);
}

InputController::~InputController()
{
	if (_input != NULL)
		this->_input->Release();
	if (_keyboard != NULL)
		this->_keyboard->Release();
}


InputController* InputController::_instance = nullptr;

InputController* InputController::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new InputController();
	}
	return _instance;
}

void InputController::release()
{
	delete _instance;
	_instance = nullptr;
}

bool InputController::init(HWND hWnd, HINSTANCE hinstance)
{
	//_keydownQueue.clear();
	//_keyupQueue.clear();
	this->_hWnd = hWnd;
	HRESULT rs;
	rs = DirectInput8Create(
		hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&_input,
		NULL);
	if (rs != DI_OK)
		return false;

	rs = _input->CreateDevice(GUID_SysKeyboard, (LPDIRECTINPUTDEVICEW*)&_keyboard, NULL);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (rs != DI_OK)
		return false;

	// Set Property for keyboard buffer.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	rs = _keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (rs != DI_OK)
		return false;

	rs = _keyboard->Acquire();
	if (rs != DI_OK)
		return false;

	return true;
}

void InputController::update()
{
	for (int i = 0; i < 256; i++)
	{
		_previousKeyBuffer[i] = ((_keyBuffer[i] & 0x80) > 0);
	}

	// collect state of all of keys.
	_keyboard->GetDeviceState(sizeof(this->_keyBuffer), _keyBuffer);
	if (isKeyDown(DIK_ESCAPE))
	{
		PostMessage(_hWnd, WM_QUIT, 0, 0);
	}

	DWORD dw = KEYBOARD_BUFFER_SIZE;
	HRESULT rs = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), _keyEvents, &dw, 0);

	int keycode, keystate;
	for (DWORD i = 0; i < dw; i++)		// not use for each statement.
	{
		keycode = _keyEvents[i].dwOfs;
		keystate = _keyEvents[i].dwData;
		if ((keystate & 0x80) > 0)
		{
			KeyEventArg* arg = new KeyEventArg(keycode);
			_keyPressed.fireEvent(arg);
			__raise __eventkeyPressed(arg);
			delete arg;
		}
		else
		{
			KeyEventArg* arg = new KeyEventArg(keycode);
			_keyReleased.fireEvent(arg);
			__raise __eventkeyReleased(arg);
			delete arg;
		}
	}
}

int InputController::isKeyDown(int keycode)
{
	return ((_keyBuffer[keycode] & 0x80) > 0);
}

bool InputController::isKeyPressed(int keycode)
{
	// Khôgn dùng được
	return isKeyDown(keycode) && !(_previousKeyBuffer[keycode]);
}

bool InputController::isKeyRelease(int keycode)
{
	// Không dùng được 
	return !isKeyDown(keycode) && (_previousKeyBuffer[keycode]);
}
