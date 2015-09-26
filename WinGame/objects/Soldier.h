#ifndef _RUNNINGSOLDIER_H
#define _RUNNINGSOLDIER_H

#include "BaseEnemy.h"
#include "IComponent.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
#include "../FrameWork/StopWatch.h"
#include <map>

using namespace std;

#define SOLDIER_SPEED 60

class Soldier : public BaseEnemy
{
public:
	Soldier() : BaseEnemy(eID::SOLDIER){ this->setStatus(RUNNING); }
	~Soldier() {};

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<eStatus, Animation*> _animations;
	StopWatch *_stopwatch;
	void changeDirection();
};
#endif