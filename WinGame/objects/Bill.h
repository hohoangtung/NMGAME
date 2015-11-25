/*
update 22/11/2015
by Luu The Vinh

- Cập nhật xét ca chạm với Land, Water
- Chuyển động nhân vật

Chưa xong:
- Chuyển độn chết
- Hiệu ứng nhảy xuông nước
- Vừa chạy vừa bắn.
- nằm -> nhảy -> rớt -> vừa rớt nhảy -> bay trong trạng thái rớt.
*/

#ifndef __BILL_H__
#define __BILL_H__

#include "..\FrameWork\define.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\Managers\SceneManager.h"
#include "..\FrameWork\StopWatch.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "Bullet.h"

#define BILL_MOVE_SPEED 120
#define BILL_JUMP_VEL 400
#define TEST_LAND 200
#define GRAVITY 800
#define SHOOT_SPEED 200.0f

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

	//void setPosition(float x, float y);

	void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);

	void onCollisionBegin(CollisionEventArg* collision_arg);
	void onCollisionEnd(CollisionEventArg* collision_arg);

	float checkCollision(BaseObject* object, float dt);

	// Đổi kiểu đạn. 
	void changeBulletType(eAirCraftType );

private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;
	
	float _movingSpeed;
	StopWatch* _stopWatch;
	bool _canJumpDown;

	void standing();
	void moveLeft();
	void moveRight();
	void jump();
	void layDown();
	void falling();
	void shoot();
	list<bool> _canStand;

	GVector2 getVelocity();
	void updateStatus(float dt);

	eStatus _currentAnimateIndex;
	void updateCurrentAnimateIndex();

	bool _sideCollide;

	list<Bullet* > _listBullets;

	eDirection getAimingDirection();
};

#endif // !__BILL_H__
