
#ifndef __BOSS_H__
#define __BOSS_H__


#include "../../FrameWork/Animation.h"
#include "../../FrameWork/StopWatch.h"
#include "../../FrameWork/Managers/SoundManager.h"
#include "../Bullets/Bullet.h"
#include "../Bullets/BulletManager.h"
#include "BaseEnemy.h"
#include "Rifleman.h"


#define BOSS_SHIELD_HP		32
#define BOSS_GUN_HP			16

#define SHIELD_SCORE		10000
#define GUN_SCORE			1000
class Boss : public BaseEnemy
{
public:
	Boss(GVector2 position, int height);
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	RECT getBounding();

	float checkCollision(BaseObject* object, float dt);

	BaseObject* getGun1();
	BaseObject* getGun2();
	BaseObject* getShield();
	BaseObject* getRifleMan();

	Sprite* _moulder1;
	Sprite* _moulder2;
	Sprite* _bigmoudler;

	Boss();
	~Boss();

	class BossGun : public BaseEnemy
	{
	public:
		// @startstatus: trạng thái bắt đầu (0 là trạng thái lồi, 1 là trạng thái thụt)
		BossGun(GVector2 position, int startStatus);
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

		// Tăng lên sau mỗi 1 giây, % 2 = 0 thì set sprite trồi, % 2 = 1 thì set sprite thụt
		int _statusGun;

		// Kiểm tra _statusGun và set lại frameRect
		void initFrameRect();
	};

	class BossBullet : public Bullet
	{
	public:

		void init();
		void update(float deltatime);
		RECT getBounding();
		BossBullet(GVector2 startposition, GVector2 force, GVector2 gravity);
		void draw(LPD3DXSPRITE, Viewport*);
		float checkCollision(BaseObject* object, float dt);
		~BossBullet();

	private:
		StopWatch* _stopWatch;
		GVector2 _startposition;
		GVector2 _force;
		GVector2 _gravity;
		BaseObject* _explosion;
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

		RECT getBounding();
		BossShield(GVector2 position);
		~BossShield();
	private :
		Animation* _animation;
		GVector2 _startposition;
		Explosion* _explosion;
	};

	class OctExplose : public BaseObject
	{
	public:
		OctExplose(GVector2);
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE, Viewport*);
		void release();
		GVector2 getPosition();		
		void reset();
	private:
		Transformable* _transform;
		BaseObject* _explosion[8];
		float _timer;

	};
private:
	GVector2 _startposition;
	int _height;
	map<string, IComponent*> _componentList;

	// Cục súng bên phải
	BaseEnemy* _gun1;

	// Cục súng bên trái
	BaseEnemy* _gun2;
	BaseEnemy* _shield;
	BaseEnemy* _rifleman;
	BaseObject* _octexplose;
};
#endif // !__BOSS_H__

