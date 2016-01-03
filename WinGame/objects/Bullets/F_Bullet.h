

#ifndef __F_BULLET__
#define __F_BULLET__

#include "Bullet.h"
#include "../../FrameWork/Animation.h"

#define F_BULLET_SPEED		200.0f
#define ROUND_RADIUS		600.0f
#define ROUND_FREQUENCE		3.5f

class FBullet : public Bullet
{
public:

	FBullet(GVector2 startPosition, float degree);
	~FBullet();


	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;

private:
	Animation* _animations;
	float initRadian();
};


#endif // !__F_BULLET__
