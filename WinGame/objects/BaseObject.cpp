
#include "BaseObject.h"
BaseObject::BaseObject(eID id)
{
	this->_id = id;
	this->setStatus(eStatus::NORMAL);
}

BaseObject::~BaseObject()
{
}

eID BaseObject::getId()
{
	return this->_id;
}

eStatus BaseObject::getStatus()
{
	return this->_status;
}

void BaseObject::setStatus(eStatus status)
{
	if(_status != status)
		_status = status;
}

void BaseObject::setPosition(GVector3 vector)
{
	Transformable::setPosition(vector);
	_sprite->setPosition(_position);
}

void BaseObject::setPosition(float x, float y, float z)
{
	Transformable::setPosition(x, y, z);

	_sprite->setPosition(_position);
}

void BaseObject::setPosition(GVector2 position)
{
	Transformable::setPosition(position);

	_sprite->setPosition(_position);
}

void BaseObject::setPosition(float x, float y)
{
	Transformable::setPosition(x, y);

	_sprite->setPosition(_position);
}

void BaseObject::setPositionX(float x)
{
	Transformable::setPositionX(x);

	_sprite->setPosition(_position);
}

void BaseObject::setPositionY(float y)
{
	Transformable::setPositionY(y);

	_sprite->setPosition(_position);
}

void BaseObject::setOrigin(GVector2 origin)
{
	Transformable::setOrigin(origin);

	_sprite->setOrigin(_origin);
}

void BaseObject::setRotate(float degree)
{
	Transformable::setRotate(degree);

	_sprite->setRotate(_rotate);
}

void BaseObject::setZIndex(int z)
{
	Transformable::setZIndex(z);

	_sprite->setZIndex(_zIndex);
}

Sprite * BaseObject::getSprite()
{
	return _sprite;
}

float BaseObject::checkCollision(BaseObject * object, float dt)
{
	return 0.0f;
}

float BaseObject::checkCollision(BaseObject * object, eDirection & direction)
{
	return 0.0f;
}

GVector2 BaseObject::getVelocity()
{
	return GVector2(0, 0);
}
