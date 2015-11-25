
#ifndef __REDCANNON_H__
#define __REDCANNON_H__

#include "BaseEnemy.h"
#include "CollisionBody.h"
#include "Bullet.h"
#include "IComponent.h"
#include "Explosion.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
#include <math.h>
#include "PlayScene.h"
using namespace std;
#define CANNON_HITPOINT 3
#define CANNON_SCORE 500
#define CANNON_SHOOTING_DELAY 1000.0f
#define CANNON_ANIMATION_SPEED 0.5f
class RedCannon : public BaseEnemy
{
public:
	RedCannon() :BaseEnemy(eID::REDCANNON){}
	~RedCannon();
	void init();
	void update(float deltatime);

	void draw(LPD3DXSPRITE spriteHandle, Viewport*);
	void release();
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);

	void destroy();
	void shoot();
	void drophitpoint();
	void setShootingAngle(float);
	float getShootingAngle();
	void setBillAngle(float);
	float getBillAngle();
	void setStatus(eStatus);

	IComponent* getComponent(string);
private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animation;
	BaseObject* _explosion;


	float _shootingAngle;
	float _billAngle;
	StopWatch* _stopwatch;
	list<Bullet*> _listBullets;

	void addStatus(eStatus);
	void removeStatus(eStatus);
	bool isInStatus(eStatus);
	void calculateBillangle();
	
};

#endif // !__REDCANNON_H__
