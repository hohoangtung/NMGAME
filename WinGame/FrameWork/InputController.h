// HOW TO USE
/*
	call InputController::getInstance()->init() at Game::init
	call InputController::getInstance()->update() at game loop.
	if you want to check if key down use:
			InputController::getInstance()->isKeydown(int keycode) at game loop or update function of an object
			With key down you can hold key to keep action.
	if you want to check if key is pressed:
			InputController::getInstance()->_keyPress += (EventFunction)&[Insert name of function here];
	if you want tỏ check if key is released:
			InputController::getInstance()->_keyReleased += (EventFunction)&[Insert name of function here];
	if you dont want to refference this function any more, call:
			InputController::getInstance()->_keyPress -= (EventFunction)&[Insert name of function here]
			InputController::getInstance()->_keyReleased -= (EventFunction)&[Insert name of function here];
	The referenced function have prototype:
		void <FunctionName>(KeyEventArg* e);
	Should call InputController::release() at Game::release()
	Example:
	void Jump(KeyEventArg* e)
	{
		if (e == NULL)
			return;
		switch (e->_keycode)
		{
			case [Space]:
			// I'm jumping....
		}
	}

	InputController::getInstance()->_keyPressed += (EventFunction) &Jump;
			And you can press space to make me jump
*/

#ifndef __INPUT_CTRL_H__
#define __INPUT_CTRL_H__

#include "define.h"
#include "Graphics.h"
#include "Event.h"
#include <vector>

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

#define KEYBOARD_BUFFER_SIZE 1024

typedef LPDIRECTINPUT			pGInput;
typedef LPDIRECTINPUTDEVICE8	pGKeyboard;

typedef void(*KeyEvent)(int);
class KeyEventArg : public EventArg
{
	// Dont need any private or protected.
public:
	KeyEventArg(int keycode){ _key = keycode; }
	int _key;
};

[event_source(native)]
class InputController
{
public:
	~InputController();

	static InputController* getInstance();
	static void release();

	bool	init(HWND, HINSTANCE);			// should be called in game::init()
	void	update();						// should be called in main game loop
	int		isKeyDown(int keycode);			// check if keycode is down.
	bool	isKeyPressed(int keycode);
	bool	isKeyRelease(int keycode);

	Event _keyPressed;
	Event _keyReleased;

	// dùng marco __event thì có thể trỏ được đến các hàm thành viên của các lớp. nhưng cách sử dụng phức tạp hơn
	__event void __eventkeyPressed(KeyEventArg* e);
	__event void __eventkeyReleased(KeyEventArg* e);

private:

	static InputController* _instance;

	//vector<KeyEvent> _keydownQueue;			// list action for keydown.
	//vector<KeyEvent> _keyupQueue;			// list action for keyup				
	pGInput		_input;
	pGKeyboard	_keyboard;
	BYTE		_keyBuffer[256];			// BYTE = unsigned char
	HWND		_hWnd;						// refference to another place. Dont release!!!
	DIDEVICEOBJECTDATA _keyEvents[KEYBOARD_BUFFER_SIZE];

	InputController();

	bool		_previousKeyBuffer[256];
};
typedef InputController *pInputController;


NS_FRAMEWORK_END
US_FRAMEWORK

#endif // !__INPUT_CTRL_H__
