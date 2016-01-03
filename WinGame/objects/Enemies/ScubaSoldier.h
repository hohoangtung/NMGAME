#ifndef _SCUBASOLDIER_H
#define _SCUBASOLDIER_H

#include "../../FrameWork/Animation.h"
#include "../../Framework/StopWatch.h"
#include "../CollisionBody.h"
#include "../IComponent.h"
#include "../Explosion.h"

#include "../Bullets/Bullet.h"
#include "../Scenes/PlayScene.h"
#include "../Bullets/BulletManager.h"
#include "../Bullets/Bullet.h"
#include "BaseEnemy.h"
using namespace std;

#define SCUBASOLDIER_HITPOINT 1
#define SCUBASOLDIER_SCORE 1000
#define SCUBASOLDIER_SHOOTING_DELAY 5050.0f
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
	RECT getBounding() override;

	class ScubaBullet : public Bullet
	{
	public:
		ScubaBullet(GVector2 startposition, GVector2 force, GVector2 gravity);
		~ScubaBullet();
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE, Viewport*);
		void release();
		float checkCollision(BaseObject* object, float dt);
	private:
		GVector2 _force;
		GVector2 _gravity;
		Explosion* _explosion;
		Explosion* _explosionburst;
		bool _tripleFlag;
		bool _canFireFlag;
		void tripleAttack();
		void fireBurst(float dt);
	};

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	Explosion *_explosion;
	list<Bullet*> _listBullets;
	bool _shot;
	bool _awake;
	float _bounce;
	float _animationTime;

	StopWatch *_stopwatch;
	StopWatch *_movingWatch;
};
#endif