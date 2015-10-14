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
#define SOLDIER_HITPOINT 1
#define SOLDIER_SCORE 100

class Soldier : public BaseEnemy
{
public:

	// ver 05.10.2015 - 7ung: đưa thân hàm constructor qua file.cpp
	// viết thân hàm ở file .h dễ bị lỗi biên dịch
	Soldier();
	~Soldier();

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	IComponent* getComponent(string);

private:
	map<string, IComponent*> _listComponent;
	map<int, Animation*> _animations;
	StopWatch *_stopwatch;
	void changeDirection();
};
#endif