#ifndef _FALCON_H
#define _FALCON_H

#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"

#include "BaseObject.h"
#include "IComponent.h"
#include "Explosion.h"
#include "Item.h"
#include "Scenes/PlayScene.h"

using namespace std;

class Falcon : public BaseObject
{
public:
	Falcon(GVector2 position, eAirCraftType type);
	Falcon(float x, float y, eAirCraftType type);
	~Falcon();

	// Các phương thức kế thừa từ BaseObject.
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	RECT getBounding() override ;

	IComponent* getComponent(string);

	// Kiểm tra va chạm.
	float checkCollision(BaseObject* object, float dt);

	eAirCraftType getType();
	void setStatus(eStatus status);
	bool isOpenned();
	void checkifOutofScreen();

private:
	
	BaseObject* _item;
	void initExplosion();
	bool isInStatus(eStatus status);

	map<string, IComponent*> _listComponent;
	Animation* _animations;
	BaseObject* _explosion;
	eAirCraftType _type;

	GVector2	_beginPosition;

};
#endif