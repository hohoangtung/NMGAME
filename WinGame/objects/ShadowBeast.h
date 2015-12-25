

#ifndef __SHADOW_BEAST__
#define __SHADOW_BEAST__

#include "BaseEnemy.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\StopWatch.h"
#include "..\FrameWork\Managers\SoundManager.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "Bazier.h"
#include "Beast_Bullet.h"

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

	struct HandElement
	{
		Sprite* _sprite;
		float _angle;
		float _lenght;
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
		RECT getBounding();
	private:
		void shoot();
		Animation* _animation;
		GVector2 _startposition;
	};

private:
	ShadowArm* _leftArm;
	ShadowArm* _rightArm;
	ShadowMouth* _mouth;
	GVector2 _position;
};


#endif // !__SHADOW_BEAST__
