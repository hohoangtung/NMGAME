#pragma once
#ifndef __REDCANNON_APPEAR__
#define __REDCANNON_APPEAR__

#include "BaseEnemy.h"
#include "BaseObject.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
#include "PlayScene.h"
#include "IComponent.h"
using namespace std;
#define REDCANNON_APPEAR_SPEED 0.1f

class RedCannon_appear : public BaseObject
{
public:
	RedCannon_appear(int type,GVector2 position);
	RedCannon_appear();
	void init();
	void update(float);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	void setPosition(GVector2);

	IComponent* getComponent(string);
	~RedCannon_appear();
private:
	map<string, IComponent*> _listComponent;
	Animation* _animation;
	int _type;
	
};
#endif

