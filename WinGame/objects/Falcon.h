#ifndef _FALCON_H
#define _FALCON_H

#include "BaseObject.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "Explosion.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>

using namespace std;

#define FALCON_FORCE			GVector2(25.0f, 225.0f)		// góc ném viên đạn
#define FALCON_GRAVITY			GVector2(0.0f, -200.0f)
#define HORIZONTAL_VELOC		GVector2(0.0f, 0.0f)		// vận tốc ngang


class Falcon : public BaseObject
{
public:
	Falcon(GVector2 position, GVector2	_horizontalVeloc, eAirCraftType type);
	Falcon(float x, float y, GVector2	_horizontalVeloc, eAirCraftType type);
	~Falcon();

	// Các phương thức kế thừa từ BaseObject.
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	GVector2 getVelocity() override;
	

	IComponent* getComponent(string);

	// Kiểm tra va chạm.
	float checkCollision(BaseObject* object, float dt);

	eAirCraftType getType();
	void setStatus(eStatus status);
	void setExplored();
	void init2();

private:

	void initExplosion();
	void updateExplosion(float deltatime);
	bool isInStatus(eStatus status);

	map<string, IComponent*> _listComponent;
	Animation* _animations;
	BaseObject* _explosion;
	eAirCraftType _type;

	GVector2	_beginPosition;
	GVector2	_force;
	bool _explored;
};
#endif