#include "EmptyObject.h"

EmptyObject::EmptyObject(eID id, GVector2 pos, int width, int height) : BaseObject(id)
{
	_sprite = nullptr;
	_bound.top = pos.y;
	_bound.left = pos.x;
	_bound.right = _bound.left + width;
	_bound.bottom = _bound.top + height;
}

EmptyObject::~EmptyObject()
{
}

GVector2 EmptyObject::getPosition()
{
	return GVector2(_bound.left, _bound.top);
}

float EmptyObject::getPositionX()
{
	return _bound.left;
}

float EmptyObject::getPositionY()
{
	return _bound.top;
}

void EmptyObject::setPosition(GVector3 vector)
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void EmptyObject::setPosition(float x, float y, float z)
{
	_bound.left = x;
	_bound.top = y;
}

void EmptyObject::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

void EmptyObject::setPosition(float x, float y)
{
	_bound.left = x;
	_bound.top = y;
}

void EmptyObject::setPositionX(float x)
{
	_bound.left = x;
}

void EmptyObject::setPositionY(float y)
{
	_bound.right = y;
}

GVector2 EmptyObject::getScale()
{
	return VECTOR2ONE;
}

void EmptyObject::setScale(GVector2 scale)
{
	// do nothing.
}

void EmptyObject::setScale(float scale)
{
	// do nothing.
}

void EmptyObject::setScaleX(float sx)
{
	// do nothing.
}

void EmptyObject::setScaleY(float sy)
{
	// do nothing.
}

GVector2 EmptyObject::getOrigin()
{
	return VECTOR2ZERO;
}

void EmptyObject::setOrigin(GVector2 origin)
{
	// do nothing.
}

float EmptyObject::getRotate()
{
	return 0.0f;
}

void EmptyObject::setRotate(float degree)
{
	// do nothing.
}

void EmptyObject::setZIndex(float z)
{
	// do nothing.
}

float EmptyObject::getZIndex()
{
	return 1;
}

GVector2 EmptyObject::getAnchorPoint()
{
	return GVector2(_bound.left, _bound.right);
}

RECT EmptyObject::getBounding()
{
	return _bound;
}