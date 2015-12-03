
#ifndef __BOSS_H__
#define __BOSS_H__


#include "BaseEnemy.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\StopWatch.h"
#include "..\FrameWork\Managers\SoundManager.h"
#include "Bullet.h"
#include "BulletManager.h"

class Boss : public BaseEnemy
{
public:
	Boss(GVector2 position);
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	RECT getBounding();

	float checkCollision(BaseObject* object, float dt);

	BaseObject* getGun1();
	BaseObject* getGun2();
	BaseObject* getShield();

	Boss();
	~Boss();

	class BossGun : public BaseEnemy
	{
	public:
		BossGun(GVector2 position);
		~BossGun();
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
		void release();

		void dropHitpoint();
		void dropHitpoint(int damage);

	private:
		vector<pair<GVector2, GVector2>> _forceBullet;
		map<string, IComponent*> _componentList;

		Animation* _animation;
		StopWatch* _stopWatch;
		BaseObject* _explosion;

		GVector2 _startposition;
	};

	class BossBullet : public Bullet
	{
	public:

		void init();
		void update(float deltatime);

		BossBullet(GVector2 startposition, GVector2 force, GVector2 gravity);
		~BossBullet();

	private:
		StopWatch* _stopWatch;
		GVector2 _startposition;
		GVector2 _force;
		GVector2 _gravity;
	};

	class BossShield : public BaseEnemy
	{
	public:
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
		void release();

		void dropHitpoint();
		void dropHitpoint(int damage);
		BossShield(GVector2 position);
		~BossShield();
	private :
		Animation* _animation;
		GVector2 _startposition;
		Explosion* _explosion;
	};
private:
	GVector2 _startposition;

	BaseEnemy* _gun1;
	BaseEnemy* _gun2;
	BaseEnemy* _shield;

};
#endif // !__BOSS_H__

