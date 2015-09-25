
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
	
	void virtual setPosition(GVector3 vector);
	void virtual setPosition(float x, float y, float z);
	void virtual setPosition(GVector2 position);
	void virtual setPosition(float x, float y);
	void virtual setPositionX(float x);
	void virtual setPositionY(float y);

	void virtual setOrigin(GVector2 origin);
	void virtual setRotate(float degree);
	void virtual setZIndex(int z);

	Sprite* getSprite();

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
