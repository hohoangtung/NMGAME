#ifndef __EMPTYOBJECT_H__
#define __EMPTYOBJECT_H__

#include "BaseObject.h"

class EmptyObject : public BaseObject
{
public:
	EmptyObject(eID id, GVector2 pos, int width, int height);
	~EmptyObject();

	virtual void init() = 0;
	virtual void update(float deltatime) = 0;
	virtual void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport) = 0;
	virtual void release() = 0;

	GVector2 getPosition() override;
	float getPositionX() override;
	float getPositionY() override;
	void setPosition(GVector3 vector) override;
	void setPosition(float x, float y, float z) override;
	void setPosition(float x, float y) override;
	void setPositionX(float x) override;
	void setPositionY(float y) override;
	void setPosition(GVector2 position) override;

	void setOrigin(GVector2 origin) override;
	GVector2 getOrigin() override;

	GVector2 getScale() override;
	void setScale(GVector2 scale) override;
	void setScale(float scale) override;
	void setScaleX(float sx) override;
	void setScaleY(float sx) override;

	float getRotate() override;
	void setRotate(float degree) override;
	GVector2 getAnchorPoint() override;

	void setZIndex(float z) override;
	float getZIndex() override;

	RECT getBounding() override;

protected:
	RECT _bound;
};

#endif // !__EMPTYOBJECT_H__
