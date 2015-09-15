#ifndef _RUNNINGSOLDER_H
#define _RUNNINGSOLDIER_H

#include "BaseObject.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>
using namespace std;

class RunningSoldier : public BaseObject
{
public:
	RunningSoldier() : BaseObject(eID::RUNNINGSOLDIER){}
	~RunningSoldier() {};

	void init();
	void update(float);
	void draw(LPD3DXSPRITE);
	void release();

	IComponent* getComponent(string);

private:
	int i;
	vector<IComponent*> _listComponent;
	map<string, IComponent*> _listComponent_;
	StopWatch* stopwatch;
	Animation* animation;
};
#endif