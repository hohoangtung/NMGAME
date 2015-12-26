
// Object: RockFall -  map 2
// Viên đá lăn xuống

#ifndef __ROCKFALL_H__
#define __ROCKFALL_H__
#include "BaseEnemy.h"
#include "CollisionBody.h"
#include "PlayScene.h"
#include "IComponent.h"
#include "Explosion.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"

#define ROCKFALL_HORIZONTAL_VELOC		GVector2(30.0f, 0.0f)		// vận tốc ngang
#define ROCKFALL_HORIZONTAL_VELOC_PRE	GVector2(-30.0f, 0.0f)		// vận tốc ngang
#define ROCKFALL_HITPOINT				4
#define ROCKFALL_GRAVITY				GVector2(0.0f, -200.0f)		// gia tốc

class RockFall : public BaseEnemy
{
public:
	RockFall(GVector2 pos);
	~RockFall();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	GVector2 getVelocity() override;

	IComponent* getComponent(string);

	void moveAround(float deltatime);				// hiệu ứng rung của hòn đá

	float checkCollision(BaseObject*, float); // kiểm tra va chạm

	void checkPosition();					// kiểm tra đối tượng trên màn hình để thiết lập thời điểm xuất hiện, thời điểm huỷ đối tượng

	void dropHitpoint();
	void dropHitpoint(int damage);

private:
	map<string, IComponent*> _listComponent;
	vector<BaseObject*> _passedLand;
	GVector2	_beginPosition;
	GVector2	_tempPosition;
	BaseObject* _explosion;
	StopWatch* _stopWatch;
	Animation* _animation;
	bool _flag;

};





#endif // !__ROCKFALL_H__
