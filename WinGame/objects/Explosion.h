

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__
#include "BaseObject.h"
#include "..\FrameWork\Animation.h"

#define SPEED_ANIMATION			0.07f
class Explosion : public BaseObject
{
public:
	Explosion(int type);
	~Explosion();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	void setPosition(GVector2 position);
	void setTimeAnimated(float);
private:
	Animation* _animation;
	int _type;
};

#endif // !__EXPLOSION_H__
