
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__
#include "..\FrameWork\define.h"
#include "..\FrameWork\SpriteManager.h"
#include "..\FrameWork\Sprite.h"
US_FRAMEWORK
class IControlable
{
public:
	virtual void updateInput(float deltatime) = 0;
};
class BaseObject
{
public:
	BaseObject(eID id);
	virtual ~BaseObject();

	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE) = 0;
protected:
	Sprite* _sprite;
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
