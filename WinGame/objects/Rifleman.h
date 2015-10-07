#ifndef _RIFLEMAN_H
#define _RIFLEMAN_H

#include "BaseEnemy.h"
#include "CollisionBody.h"
#include "Bullet.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

using namespace std;

#define RIFLEMAN_HITPOINT 1
#define RIFLEMAN_SCORE 500
#define RIFLEMAN_SHOOTING_DELAY 1050.0f
#define RIFLEMAN_ANIMATION_SPEED 0.4755f

class Rifleman : public BaseEnemy
{
public:
	Rifleman();
	~Rifleman();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	float getShootingAngle();
	void setShootingAngle(double);

	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);

	void die();
	void shoot();
	
	void setStatus(eStatus);
	
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	float _shootingAngle;
	StopWatch *_stopwatch;

	list<Bullet*> _listBullets;

	void addStatus(eStatus status);
	void removeStatus(eStatus status);
	bool isInStatus(eStatus status);
};
#endif