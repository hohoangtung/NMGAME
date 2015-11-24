#pragma once
#ifndef __WALL_TURRET_H__
#define __WALL_TURRET_H__
#include "BaseEnemy.h"
#include "CollisionBody.h"
#include "IComponent.h"
#include "Bullet.h"
#include "Explosion.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
#include <math.h>
#define WALL_TURRET_HITPOINT 1
#define WALL_TURRET_SCORE 1000
#define WALL_TURRET_SHOOTING_DELAY 1000.0f
#define WALL_TURRET_ANIMATION_SPEED 0.5f
using namespace std;
class WallTurret :public BaseEnemy
{
public:
	WallTurret() :BaseEnemy(eID::WALL_TURRET){}

	~WallTurret();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);

	void setBillAngle(float);
	float getBillAngle();
	void setShootingAngle(float);
	float getShootingAngle();

	void setStatus(eStatus);
	void shoot();
	void destroy();

	IComponent* getComponent(string);
private:
	map < string, IComponent*> _listComponent;
	map<int, Animation*> _animation;
	float _shootingAngle;
	float _billAngle;
	BaseObject* _explosion;
	list<Bullet*> _listBullet;
	StopWatch* _stopwatch;

	void addStatus(eStatus);
	void removeStatus(eStatus);
	bool isInStatus(eStatus);
	void calculateBillangle();
};
#endif
