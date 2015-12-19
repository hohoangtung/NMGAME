/*
update 22/11/2015
by Luu The Vinh

- Cập nhật xét ca chạm với Land, Water
- Chuyển động nhân vật

update 30/11/2015
- Hiệu ứng nhảy xuông nước
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
#include "L_Bullet.h"
#include "F_Bullet.h"
#include "S_Bullet.h"
#include "M_Bullet.h"
#include "LifeUI.h"

#define BILL_MOVE_SPEED 120
#define BILL_JUMP_VEL 450
#define GRAVITY 800
#define SHOOT_SPEED 200.0f
#define MAX_BULLET 4
#define REVIVE_TIME 2000
#define PROTECT_TIME 3000

[event_receiver(native)]
class Bill : public BaseObject, public IControlable
{
public:
	Bill(int life = 3);
	~Bill();

	void init();
	void updateInput(float dt);
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();

	void onKeyPressed(KeyEventArg* key_event);
	void onKeyReleased(KeyEventArg* key_event);

	void onCollisionBegin(CollisionEventArg* collision_arg);
	void onCollisionEnd(CollisionEventArg* collision_arg);

	float checkCollision(BaseObject* object, float dt);
	void checkPosition();

	// Đổi kiểu đạn. 
	void changeBulletType(eAirCraftType );
	void die();

	void setLifeNumber(int number);
	int getLifeNumber();

	void setShootSpeed(float speed);
	float getShootSpeed();

	void setMaxBullet(int number);
	int getMaxBullet();

	void setStatus(eStatus status) override;

	RECT getBounding() override;

private:
	map<int, Animation*> _animations;
	map<string, IComponent*> _componentList;
	
	float _movingSpeed;
	StopWatch* _stopWatch;
	StopWatch* _shootStopWatch;
	StopWatch* _reviveStopWatch;

	bool _canJumpDown;
	eStatus _currentAnimateIndex;

	list<Bullet* > _listBullets;

	// Dùng để tạo ra đạn, nếu ăn máy bay tiếp đạn thì thay đổi thông số này, nếu bắn đạn thì dựa trên thuộc tính này để chọn loại đạn khởi tạo
	eBulletType _currentGun;

	int _lifeNum;
	float _shootSpeed;
	int _maxBullet;
	float _protectTime;

	LifeUI* _lifeUI;

	void standing();
	void moveLeft();
	void moveRight();
	void jump();
	void layDown();
	void falling();
	void shoot();
	void revive();

	GVector2 getVelocity();

	void updateStatus(float dt);

	void updateCurrentAnimateIndex();

	eDirection getAimingDirection();

	Bullet* getBulletFromGun(GVector2 position, float angle);

	// Tung Ho: kiểm tra và xoá đạn hết hiệu lực.
	void deleteBullet();
};

#endif // !__BILL_H__
