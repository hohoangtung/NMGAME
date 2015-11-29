

#ifndef __M_BULLET__
#define __M_BULLET_

#include "Bullet.h"

#define M_BULLET_SPEED 500.0f

class MBullet : public Bullet
{
public:
	MBullet(GVector2 startPosition, float degree);
	~MBullet();

	void init() override;

private:

};


#endif // __M_BULLET__
