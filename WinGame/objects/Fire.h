#ifndef __FIRE__
#define __FIRE__
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
using namespace std;

#define HORIZONTAL_VELOC		GVector2(100.0f, 0.0f)		// vận tốc ngang
#define HORIZONTAL_VELOC_PRE	GVector2(-100.0f, 0.0f)		// vận tốc ngang

class Fire:public BaseObject
{
public:
	Fire(GVector2,GVector2,float x,float y);

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	GVector2 getVelocity();
	IComponent* getComponent(string);
	~Fire();
private:
	map<string, IComponent*> _listComponent;
	GVector2 _leftPosition;
	GVector2 _rightPosition;

};
#endif
