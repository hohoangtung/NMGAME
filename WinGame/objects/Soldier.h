#ifndef _RUNNINGSOLDIER_H
#define _RUNNINGSOLDIER_H

#include "BaseObject.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
using namespace std;

class Soldier : public BaseObject
{
public:
	Soldier() : BaseObject(eID::SOLDIER){}
	~Soldier() {};

	void init();
	void update(float);
	void draw(LPD3DXSPRITE);
	void release();

	IComponent* getComponent(string);

private:
	int i;
	map<string, IComponent*> _listComponent;
	map<eStatus, Animation*> _animations;
	StopWatch *_stopwatch;
	// Animation* _animation;
};
#endif