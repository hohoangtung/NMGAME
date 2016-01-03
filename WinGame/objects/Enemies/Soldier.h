#ifndef _RUNNINGSOLDIER_H
#define _RUNNINGSOLDIER_H

#include "../../FrameWork/Animation.h"
#include "../../FrameWork/StopWatch.h"
#include "../Explosion.h"
#include "../IComponent.h"
#include "../Scenes/PlayScene.h"
#include "BaseEnemy.h"

using namespace std;

#define SOLDIER_SPEED 120
#define SOLDIER_HITPOINT 1
#define SOLDIER_SCORE 100
#define SOLDIER_JUMP_VELOCITY 250
#define SOLDIER_SHOOTING_DELAY 400.0f

class Soldier : public BaseEnemy
{
public:
	/* 
		@status: Tạm thời chỉ truyền RUNNING
		@pos: Vị trí
		@direction: hướng chạy, -1 là bên trái, 1 là bên phải
		@x, y: tọa độ của pos
		@shoot: có cho lính bắn dc hay không
	*/
	Soldier(eStatus status, GVector2 pos, int direction);
	Soldier(eStatus status, float x, float y, int direction);
	Soldier(eStatus status, GVector2 pos, int direction, bool shoot);
	Soldier(eStatus status, float x, float y, int direction, bool shoot);
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
	void setPosition(GVector2);
	GVector2 getVelocity();
	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	StopWatch *_stopwatch;
	StopWatch *_shoot;
	StopWatch *_loopwatch;
	StopWatch *_checkShoot;
	BaseObject *_explosion;
	BaseObject* prevObject;
	list<Bullet*> _listBullets;
	Sprite *_divingSprite;
	bool _canShoot;
	
	void shoot();
	void changeDirection();
};
#endif