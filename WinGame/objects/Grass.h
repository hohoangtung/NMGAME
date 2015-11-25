

#ifndef __GRASS_H__
#define __GRASS_H__

#include "..\FrameWork\define.h"
#include "..\FrameWork\Animation.h"
#include "..\FrameWork\InputController.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "Bullet.h"
 
class Grass : public BaseObject
{
public:
	Grass(GVector2 pos);
	~Grass();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
private:
	map<string, IComponent*> _componentList;
};


#endif // !__GRASS_H__

