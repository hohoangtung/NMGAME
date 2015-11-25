#ifndef _RUNNINGSOLDIER_H
#define _RUNNINGSOLDIER_H

#include "BaseEnemy.h"
#include "Explosion.h"
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
	/* 
		@status: Tạm thời chỉ truyền RUNNING
		@pos: Vị trí
		@direction: hướng chạy, -1 là bên trái, 1 là bên phải
		@x, y: tọa độ của pos
	*/
	Soldier(eStatus status, GVector2 pos, int direction);
	Soldier(eStatus status, float x, float y, int direction);
	~Soldier();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);
	void jump();
	void die();
	GVector2 getVelocity();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	StopWatch *_stopwatch;
	void changeDirection();
	BaseObject *_explosion;
	BaseObject* prevObject;
};
#endif