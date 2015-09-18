#ifndef __BILL_H__
#define __BILL_H__

#include "..\FrameWork\define.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\InputController.h"
#include "BaseObject.h"
#include "IComponent.h"

#define BILL_MOVE_SPEED 50
#define BILL_JUMP_VEL 200
#define BILL_ACC_MOVE 300
#define TEST_LAND 200
#define GRAVITY 50

[event_receiver(native)]
class Bill : public BaseObject, public IControlable
{
public:
	Bill();
	~Bill();

	void init();
	void updateInput(float dt);
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();

	void setPosition(float x, float y);

	void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);

private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;

	void standing();
	void moveLeft();
	void moveRight();
	void jump();
	void layDown();

	void setState(int state);
	void addState(int state);
	void removeState(int state);
	bool isInState(int state);
	int getState();
	int _state;

	GVector2 getVelocity();
	void updateStatus(float dt);

	int _currentAnimateState;
	void updateCurrentState();
};

#endif // !__BILL_H__
