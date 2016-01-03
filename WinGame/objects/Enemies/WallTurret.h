#pragma once
#ifndef __WALL_TURRET_H__
#define __WALL_TURRET_H__

#include "../../FrameWork/Animation.h"
#include "../../FrameWork/StopWatch.h"
#include "../Scenes/PlayScene.h"
#include "../Bullets/Bullet.h"
#include "../CollisionBody.h"
#include "../IComponent.h"
#include "../Explosion.h"
#include "BaseEnemy.h"

#define WALL_TURRET_HITPOINT 8
#define WALL_TURRET_SCORE 1000
#define WALL_TURRET_SHOOTING_DELAY 3000.0f
#define WALL_TURRET_ANIMATION_SPEED 0.3f
#define WALL_TURRET_APPEAR_SPEED 0.1f
using namespace std;
class WallTurret :public BaseEnemy
{
public:
	WallTurret(eStatus status, GVector2 pos);
	WallTurret(eStatus status, float x, float y);
	WallTurret(eWT_Status wtstatus, GVector2 pos);
	WallTurret(eWT_Status wtstatus, float x, float y);

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
	void setWTStatus(eWT_Status);
	eWT_Status getWT_Status();
	void shoot();
	RECT getBounding();
	
	/*float checkCollision(BaseObject*, float);*/
	void checkIfOutofScreen();
	IComponent* getComponent(string);
private:
	map < string, IComponent*> _listComponent;
	map<int, Animation*> _animation;
	float _shootingAngle;
	float _billAngle;
	eWT_Status _wtstatus;
	BaseObject* _explosion;
	
	//list<Bullet*> _listBullet;
	StopWatch* _stopwatch;
	
	void initExplosion();
	void updateExplosion(float);

	void addStatus(eStatus);
	void addStatus(eWT_Status);
	void removeStatus(eStatus);
	void removeStatus(eWT_Status);
	bool isInStatus(eStatus);
	bool isInStatus(eWT_Status);
	void calculateBillangle();

	void checkBill();
	void rangeAttack();
	bool isRange();
};
#endif
