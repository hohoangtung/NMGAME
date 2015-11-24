#ifndef __BULLET_H__
#define __BULLET_H__

#include "BaseObject.h"
#include "IComponent.h"
#include "../FrameWork/define.h"
#include "CollisionBody.h"


#define NORMAL_BULLET_SPEED 400;

class Bullet : public BaseObject
{
public:
	/*
	Khởi tạo viên đạn
		@startPosition: vị trí bắt đầu
		@dir: hướng viên đạn
	*/
	Bullet(GVector2 startPosition, eDirection dir);

	/*
	Khởi tạo viên đạn
		@startPosition: vị trí bắt đầu
		@degree: hướng viên đạn theo độ, gốc là 12h, theo chiều kim đồng hồ
	*/
	Bullet(GVector2 startPosition, float degree);

	~Bullet();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE, Viewport*);
	virtual void release();

	virtual int getDamage();

	GVector2 getVelocity();

	void onCollisionBegin(CollisionEventArg* collision_arg);

	float checkCollision(BaseObject* object, float dt);

protected:
	GVector2 _startPosition;
	float _degree;

	int _damage;
	eDirection _direction;

private:
	map<string, IComponent*> _componentList;

};

#endif // !__BULLET_H__
