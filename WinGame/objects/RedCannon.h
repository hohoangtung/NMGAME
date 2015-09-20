
#ifndef __REDCANNON_H__
#define __REDCANNON_H__

#include "BaseObject.h"
#include "..\Game.h"
class RedCannon : public BaseObject
{
public:
	RedCannon(GVector2 position);
	~RedCannon();
	void init();
	void update(float deltatime);

	void draw(LPD3DXSPRITE spriteHandle, Viewport*);
	void release() {}
private:

};

#endif // !__REDCANNON_H__
