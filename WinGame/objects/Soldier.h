#ifndef _RUNNINGSOLDIER_H
#define _RUNNINGSOLDIER_H

#include "BaseEnemy.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>

using namespace std;

#define SOLDIER_SPEED 60
#define SOLDIER_HITPOINT 1
#define SOLDIER_SCORE 100
#define SOLDIER_JUMP_VELOCITY 200

class Soldier : public BaseEnemy
{
public:
	Soldier();
	~Soldier();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);
	void jump();
	GVector2 getVelocity();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	StopWatch *_stopwatch;
	void changeDirection();
};
#endif