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

	//void setState(int state);
	void addStatus(eStatus status);
	void removeStatus(eStatus status);
	bool isInStatus(eStatus status);
	
	GVector2 getVelocity();
	void updateStatus(float dt);

	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();
};

#endif // !__BILL_H__
