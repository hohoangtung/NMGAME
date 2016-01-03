#ifndef __BULLET_H__
#define __BULLET_H__

#include "../../FrameWork/define.h"
#include "../../FrameWork/Managers/SceneManager.h"
#include "../../FrameWork/StopWatch.h"
#include "../Enemies/BaseEnemy.h"
#include "../BaseObject.h"
#include "../IComponent.h"
#include "../CollisionBody.h"
#define NORMAL_BULLET_SPEED 400

#define TOP_SHOOT_ANGLE					0.0f
#define RIGHT_SHOOT_ANGLE				90.0f
#define LEFT_SHOOT_ANGLE				-90.0f
#define BOTTOM_SHOOT_ANGLE				180.0f

#define TOPRIGHT_SHOOT_ANGLE			60.0f
#define TOPLEFT_SHOOT_ANGLE				-60.0f
#define BOTRIGHT_SHOOT_ANGLE			120.0f
#define BOTLEFT_SHOOT_ANGLE				-120.0f

class Bullet : public BaseObject
{
public:
	/*
	Khởi tạo viên đạn
		@startPosition: vị trí bắt đầu
		@type: loại đạn, chia ra đạn của bill (= 0) và enemy (= 1).
		tuy nhiên còn có các loại đạn khác, nên lúc gán thì gán (BILL_BULLET | SOMETYPE) hoặc (ENEMY_BULLET | SOMETYPE)
		@dir: hướng viên đạn
	*/
	Bullet(GVector2 startPosition,  eBulletType type, eDirection dir);

	/*
	Khởi tạo viên đạn
		@startPosition: vị trí bắt đầu
		@type: loại đạn, chia ra đạn của bill (= 1) và enemy (= 2).
		tuy nhiên còn có các loại đạn khác, nên lúc gán thì gán (BILL_BULLET | SOMETYPE) hoặc (ENEMY_BULLET | SOMETYPE)
		@degree: hướng viên đạn theo độ, gốc là 12h, theo chiều kim đồng hồ
	*/
	Bullet(GVector2 startPosition, eBulletType type, float degree);

	virtual ~Bullet();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE, Viewport*);
	virtual void release();

	virtual int getDamage();
	virtual void setDamge(int dmg);

	GVector2 getVelocity();
	
	eBulletType getBulletType();
	float getDegree();
	bool isBillBullet();
	bool isEnemyBullet();
	bool isContainType(eBulletType type);
	void onCollisionBegin(CollisionEventArg* collision_arg);

	virtual float checkCollision(BaseObject* object, float dt);

protected:
	GVector2 _startPosition;
	float _degree;

	int _damage;
	eDirection _direction;
	eBulletType _type;
	GVector2 initveloc(float bullet_speed);

	map<string, IComponent*> _componentList;
	StopWatch* _bursttime;

private:
	//map<string, IComponent*> _componentList;

};

#endif // !__BULLET_H__
