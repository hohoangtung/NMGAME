#ifndef _RIFLEMAN_H
#define _RIFLEMAN_H

#include "BaseEnemy.h"
#include "../FrameWork/Animation.h"
#include "../Framework/StopWatch.h"

using namespace std;

#define RIFLEMAN_HITPOINT 1
#define RIFLEMAN_SCORE 500

class Rifleman : public BaseEnemy
{
public:
	Rifleman() : BaseEnemy(eID::RIFLEMAN){ this->setStatus(NORMAL); }
	~Rifleman() {};

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	double getShootingAngle();
	void setShootingAngle(double);

	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	double _shootingAngle;
	StopWatch *_stopwatch;

	void addStatus(eStatus status);
	void removeStatus(eStatus status);
	bool isInStatus(eStatus status);
};
#endif