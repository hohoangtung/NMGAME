#ifndef __ROCKFLY_H__
#define __ROCKFLY_H__

#include "../FrameWork/Animation.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "Scenes/PlayScene.h"


#define ROCKFLY_HORIZONTAL_VELOC		GVector2(96.0f, 0.0f)		// vận tốc ngang
#define ROCKFLY_HORIZONTAL_VELOC_PRE	GVector2(-96.0f, 0.0f)		// vận tốc ngang

class RockFly : public BaseObject
{
public:
	// leftPos: vị trí ngoài cùng bên trái 
	// rightPos: vị trí ngoài cùng bên phải
	RockFly(GVector2 leftPos, GVector2 rightPos);
	~RockFly();

	// Các phương thức kế thừa từ BaseObject.
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	GVector2 getVelocity() override;

	IComponent* getComponent(string componentName);
	RECT getBounding() override;

private:
	map<string, IComponent*> _listComponent;

	GVector2	_leftPosition;
	GVector2	_rightPosition;

	void checkPosition();

};


#endif // !__ROCKFLY_H__
