

#ifndef __ROCK_CREATOR__
#define __ROCK_CREATOR__

#include "../../FrameWork/StopWatch.h"
#include "../BaseObject.h"
#include "../Scenes/Stage3.h"
#include "RockFall.h"

class RockCreator : public BaseObject
{
public:
	RockCreator(GVector2 position);
	~RockCreator();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	RECT getBounding() override;
	float checkCollision(BaseObject * object, float dt);
	BaseObject* getRock();
private:
	GVector2 _position;
	RockFall* _rockInstance;
	StopWatch* _stopwatch;
};

#endif // !__ROCK_CREATOR__
