#ifndef _FALCON_H
#define _FALCON_H

#include "BaseObject.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
using namespace std;

class Falcon : public BaseObject
{
public:
	Falcon() : BaseObject(eID::FALCON){}
	~Falcon() {};

	void init();
	void update(float);
	void draw(LPD3DXSPRITE);
	void release();

	IComponent* getComponent(string);

private:
	int i;
	map<string, IComponent*> _listComponent;
	StopWatch* _stopwatch;
	Animation* _animation;
};
#endif