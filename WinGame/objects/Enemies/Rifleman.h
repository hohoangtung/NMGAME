#ifndef _RIFLEMAN_H
#define _RIFLEMAN_H

#include "../../FrameWork/Animation.h"
#include "../../Framework/StopWatch.h"
#include "../Bullets/BulletManager.h"
#include "../Bullets/Bullet.h"
#include "../Scenes/PlayScene.h"
#include "../CollisionBody.h"
#include "../IComponent.h"
#include "BaseEnemy.h"
using namespace std;

#define RIFLEMAN_HITPOINT 1
#define RIFLEMAN_SCORE 500
#define RIFLEMAN_SHOOTING_DELAY 250.0f
#define RIFLEMAN_BULLET_BURST_DELAY 3050.0f
#define RIFLEMAN_ANIMATION_SPEED 0.4755f

class Rifleman : public BaseEnemy
{
public:
	/*
		@status: NORMAL cho lính đứng bắn, HIDDEN cho lính núp
		@pos: vị trí
		@x, y: tọa độ của pos
	*/
	Rifleman(eStatus status, GVector2 pos);
	Rifleman(eStatus status, float x, float y);
	~Rifleman();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float getShootingAngle();
	void setShootingAngle(double);

	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);

	void die();
	void shoot();
	
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	float _shootingAngle;
	StopWatch *_stopwatch;
	StopWatch *_loopwatch;
	StopWatch *_shootingWatch;
	BaseObject *_explosion;

	void calculateShootingAngle();
	void calculatingShootingDirection();
	void changeShootingStance();
};
#endif