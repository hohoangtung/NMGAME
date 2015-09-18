#ifndef __BILL_H__
#define __BILL_H__

#include "..\FrameWork\define.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\InputController.h"
#include "BaseObject.h"
#include <olebind.h>

#define BILL_MOVE_SPEED 50
#define BILL_JUMP_VEL 150
#define BILL_ACC_MOVE 300
#define TEST_LAND 200
#define GRAVITY 50

enum eGravityStatus
{
	FALLING__DOWN,
	SHALLOWED
};

class Gravity : public IComponent
{
public:
	// khởi tạo trọng lực của đối tượng, tham chiếu đến movement conponent của đối tượng
	Gravity(GVector2 gravity, Movement *movement)
	{
		this->_gravity = gravity;
		this->_refmovement = movement;
		this->_status = FALLING__DOWN;
	}
	// khi muốn nhảy. set lại status cho gravity là FALLING_DOWN
	// khi va chạm với đất set lại status cho gravity là  SHALLOWED
	void setStatus(eGravityStatus status)
	{
		this->_status = status;
	}
	void update(float deltatime)
	{
		switch (_status)
		{
		case FALLING__DOWN:
			this->_additionalVeloc += this->_gravity * deltatime / 1000;
			break;
		case SHALLOWED:
			this->_additionalVeloc = GVector2(0.0f, 0.0f);
		default:
			break;
		}
		auto veloc = this->_refmovement->getVelocity();
		this->_refmovement->setVelocity(veloc + _additionalVeloc);
	}
private:
	GVector2 _gravity;
	GVector2 _additionalVeloc;
	Movement* _refmovement;
	eGravityStatus _status;
};

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

	static void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);

private:
	map<eStatus, Animation*> _animations;
	map<string, IComponent*> _componentList;

	void standing();
	void moveLeft();
	void moveRight();
	void jump();
	void layDown();

	GVector2 getVelocity();
};

#endif // !__BILL_H__
