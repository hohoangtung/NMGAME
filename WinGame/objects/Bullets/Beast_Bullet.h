

#ifndef __BEAST_BULLET_H__
#define __BEAST_BULLET_H__


#include "Bullet.h"
#include "../../FrameWork/Animation.h"

#define BEAST_BULLET_SPEED 240.0f

class BeastBullet : public Bullet
{
public:
	BeastBullet(GVector2 startposition, float degree);
	~BeastBullet();
	void init();
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE spriteHandle, Viewport *viewport) override;
	RECT getBounding() override;
private:
	Animation* _animation;
};

#endif // !__BEAST_BULLET_H__
