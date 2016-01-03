

#ifndef __SHADOW_BEAST__
#define __SHADOW_BEAST__

#include "../../FrameWork/Animation.h"
#include "../../FrameWork/StopWatch.h"
#include "../../FrameWork/Managers/SoundManager.h"
#include "../Bullets/Bullet.h"
#include "../Bullets/BulletManager.h"
#include "../Bullets/Beast_Bullet.h"
#include "BaseEnemy.h"

class ShadowBeast : public BaseEnemy
{
public:
	ShadowBeast(GVector2 position);
	~ShadowBeast();
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	RECT getBounding();
	float checkCollision(BaseObject* object, float dt);

	BaseObject* getLeftArm();
	BaseObject* getRigtArm();
	BaseObject* getMouth();

	struct HandElement
	{
		Sprite* _sprite;
		float _angle;
		float _lenght;
		HandElement(Sprite* sprite, float angle, float lenght)
		{
			_sprite = sprite;
			_angle = angle;
			_lenght = lenght;
		}
		HandElement()
		{
			_sprite = nullptr;
			_angle = 0.0f;
			_lenght = 0.0f;
		}
	};

	class ShadowArm : public BaseEnemy
	{
	public:
		ShadowArm(GVector2 root);
		~ShadowArm();
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
		void release();
		RECT getBounding();
		float checkCollision(BaseObject* object, float dt);
		void initComponent(Sprite* sprite);
		void roundCircle(float deltatime);
		void updateElemPosition(HandElement& elem);
		void wave(float deltatime);
		void aimAttack(float deltatime);
		void setWise(bool);

		
	private:
		GVector2 _rootOfArm;
		float roundCircletime;
		GVector2** _anchorGroups;
		map<string, IComponent*> _componentList;

		StopWatch* _stopWatch;
		StopWatch* _aimtime;
		StopWatch* _trackBillTime;

		HandElement _handelem1;
		HandElement _handelem2;
		HandElement _handelem3;
		HandElement _handelem4;
		HandElement _arm;

		Explosion* _explosion;
		static void calculateLenght(HandElement& currentElem, HandElement& prevElem);
		bool _flagAimFinish[4];
		bool _isAiming;
		bool flagforTrackBill;
		int counterforFire;
		int _status;
		int _waveCount;
		bool _isClockWise;
		void changeWise();

		void shoot();
		void updateStatusBurst(float dt);
		void updateStatusNormal(float dt);
		enum Status
		{
			normal,
			waving,
			aiming
		}_memStatus;
	};

	class ShadowMouth :public BaseEnemy
	{
	public:
		ShadowMouth(GVector2 startposition);
		~ShadowMouth();
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
		void release();
		eMouthStatus getMouthStatus();
		void setMouthStatus(eMouthStatus);
		RECT getBounding();
	private:
		void shoot();
		map<int, Animation*> _animations;
		Animation* _animation;
		Explosion* _explosion;
		GVector2 _startposition;
		eMouthStatus _mouthStatus;
		bool _flagCanShoot;

		void updateStatusNormal(float deltatime);
		void updateStatusBurst(float deltatime);
		bool _flagBigExplosion;

		void drawBillExplosion(LPD3DXSPRITE spriteHandle, Viewport* viewport);
		static GVector2 explose_position[12];
		bool _flagMoudle;
		Sprite* _moulder;

	};

private:
	ShadowArm* _leftArm;
	ShadowArm* _rightArm;
	ShadowMouth* _mouth;
	GVector2 _position;
	bool _flagPlayedDestroyBoss;
};


#endif // !__SHADOW_BEAST__
