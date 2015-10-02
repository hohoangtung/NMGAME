
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__
#include "..\FrameWork\define.h"
#include "..\FrameWork\SpriteManager.h"
#include "..\FrameWork\Sprite.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\Viewport.h"

US_FRAMEWORK
class IControlable
{
public:
	IControlable()
	{
		_input = InputController::getInstance();
	}
	virtual void updateInput(float deltatime) = 0;
protected:
	InputController* _input;
};

class BaseObject : public Transformable
{
public:
	BaseObject(eID id);
	virtual ~BaseObject();

	virtual void init() = 0;
	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE, Viewport*) = 0;
	virtual void release() = 0;

	eID getId();
	eStatus getStatus();
	virtual void setStatus(eStatus status);
	
	virtual void setPosition(GVector3 vector);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(GVector2 position);
	virtual void setPosition(float x, float y);
	virtual void setPositionX(float x);
	virtual void setPositionY(float y);

	virtual void setOrigin(GVector2 origin);
	virtual void setRotate(float degree);
	virtual void setZIndex(int z);

	Sprite* getSprite();

	virtual float checkCollision(BaseObject* object, float dt);
	virtual float checkCollision(BaseObject* object, eDirection& direction);

	// vật nào cũng có vận tốc, mặc định là đứng yên sẽ return 0
	// nếu vật có chuyển động phải override lại để lấy vận tốc.
	virtual GVector2 getVelocity();

protected:
	Sprite* _sprite;
	// nếu muốn dùng animation thì class kế thừa khai báo animation
	// khởi tạo anition thì reference đến sprite
	eID _id;
	eStatus _status;

	
};
/*

class Amry : public BaseObject : public IControlable
{
public:
	Amry(): BaseObject(eID::ARMY);
	void updateInput(float deltatime) override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE) override;
}
*/
#endif // !__BASEOBJECT_H__
