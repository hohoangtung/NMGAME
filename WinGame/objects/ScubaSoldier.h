#ifndef _SCUBASOLDIER_H
#define _SCUBASOLDIER_H

#include "BaseEnemy.h"
#include "CollisionBody.h"
#include "Bullet.h"
#include "IComponent.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"
#include "PlayScene.h"
#include "BulletManager.h"
using namespace std;

#define SCUBASOLDIER_HITPOINT 1
#define SCUBASOLDIER_SCORE 1000
#define SCUBASOLDIER_SHOOTING_DELAY 1050.0f
#define SCUBASOLDIER_ANIMATION_SPEED 0.4755f

class ScubaSoldier: public BaseEnemy
{
public:
	ScubaSoldier(GVector2 pos);
	ScubaSoldier(float x, float y);
	~ScubaSoldier();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);
	float checkCollision(BaseObject*, float);

	void die();
	void shoot();

	IComponent* getComponent(string);
private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	Explosion *_explosion;
	list<Bullet*> _listBullets;

	StopWatch *_stopwatch;
};
#endif