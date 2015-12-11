#pragma once
#ifndef __WALLTURRET_INACTIVED__
#define _WALLTURRET_INACTIVED__

#include "BaseEnemy.h"
#include "BaseObject.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
#include "PlayScene.h"
#include "IComponent.h"
using namespace std;

#define WALLTURRET_INACTIVED_SPEED 0.1f

class WallTurret_inactived :public BaseObject
{
public:
	WallTurret_inactived(int,GVector2);
	WallTurret_inactived(int,float x, float y);

	void init();
	void update(float);
	void draw(LPD3DXSPRITE spritehandle, Viewport* viewport);
	void release();
	void setPosition(GVector2);

	IComponent* getComponent(string);
	~WallTurret_inactived();
private:
	map<string, IComponent*> _listComponent;
	Animation* _animation;
	int _type;
};
#endif

