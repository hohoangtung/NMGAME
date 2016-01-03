

#ifndef __ITEM__
#define __ITEM__

#include "../FrameWork/Animation.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "Scenes/PlayScene.h"

#define ITEM_FORCE				GVector2(50.0f, 250.0f)	
#define ITEM_GRAVITY			GVector2(0.0f, -250.0f)


class Item : public BaseObject
{
public:
	Item(GVector2 position, eAirCraftType type);
	~Item();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	GVector2 getVelocity() override;
	RECT getBounding() override;

	void checkifOutofScreen();

	// Kiểm tra va chạm.
	float checkCollision(BaseObject* object, float dt);
private:
	map<string, IComponent*> _listComponent;
	eAirCraftType _type;
	GVector2 _startposition;
	Animation* _animation;

	static RECT getFrameRectFromType(eAirCraftType type);
};

#endif // !__ITEM__
