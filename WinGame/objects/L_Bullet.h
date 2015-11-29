

#ifndef __L_BULLET__
#define __L_BULLET__

#include "Bullet.h"

#define L_BULLET_SPEED 480.0f




class LGun : public Bullet
{
public:
	LGun(GVector2 startPosition, float degree);
	~LGun();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	float checkCollision(BaseObject* object, float dt) override;

	class LBullet : public Bullet
	{
	public:

		LBullet(GVector2 startPosition, float degree);
		~LBullet();


		void init() override;
		void update(float deltatime) override;
		void draw(LPD3DXSPRITE, Viewport*) override;
		void release() override;

	private:

	};
	static GVector2 generatePosition(GVector2 startposition, float degree, float R, float H);
private:
	Bullet* _childBullet[3];

	void deleteChilds();

	Bullet* initChildBullet(GVector2 startposition, float _degree);
};


#endif // !__L_BULLET__
