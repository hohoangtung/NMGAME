
#include "BaseObject.h"
BaseObject::BaseObject(eID id)
{
	this->_id = id;
	this->setStatus(eStatus::NORMAL);
	_physicsType = ePhysicsBody::NOTHING;
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

void BaseObject::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}

void BaseObject::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}

bool BaseObject::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;;
}

GVector2 BaseObject::getPosition()
{
	return _sprite->getPosition();
}

float BaseObject::getPositionX()
{
	return _sprite->getPositionX();
}

float BaseObject::getPositionY()
{
	return _sprite->getPositionY();
}

void BaseObject::setPosition(GVector3 vector)
{
	_sprite->setPosition(vector);
}

void BaseObject::setPosition(float x, float y, float z)
{
	_sprite->setPosition(x, y, z);
}

void BaseObject::setPosition(GVector2 position)
{
	_sprite->setPosition(position);
}

void BaseObject::setPosition(float x, float y)
{
	_sprite->setPosition(x, y);
}

void BaseObject::setPositionX(float x)
{
	_sprite->setPositionX(x);
}

void BaseObject::setPositionY(float y)
{
	_sprite->setPositionY(y);
}

GVector2 BaseObject::getScale()
{
	return _sprite->getScale();
}

void BaseObject::setScale(GVector2 scale)
{
	_sprite->setScale(scale);
}

void BaseObject::setScale(float scale)
{
	_sprite->setScale(scale);
}

void BaseObject::setScaleX(float sx)
{
	_sprite->setScaleX(sx);
}

void BaseObject::setScaleY(float sy)
{
	_sprite->setScaleY(sy);
}

GVector2 BaseObject::getOrigin()
{
	return _sprite->getOrigin();
}

void BaseObject::setOrigin(GVector2 origin)
{
	_sprite->setOrigin(origin);
}

float BaseObject::getRotate()
{
	return _sprite->getRotate();
}

void BaseObject::setRotate(float degree)
{
	_sprite->setRotate(degree);
}

void BaseObject::setZIndex(int z)
{
	_sprite->setZIndex(z);
}

int BaseObject::getZIndex()
{
	return _sprite->getZIndex();
}

GVector2 BaseObject::getAnchorPoint()
{
	return _sprite->getAnchorPoint();
}

Sprite * BaseObject::getSprite()
{
	return _sprite;
}

RECT BaseObject::getBounding()
{
	return _sprite->getBounding();
}

float BaseObject::checkCollision(BaseObject * object, float dt)
{
	return 0.0f;
}

GVector2 BaseObject::getVelocity()
{
	return GVector2(0, 0);
}

void BaseObject::setPhysicsBodyType(ePhysicsBody type)
{
	if (type != _physicsType)
		_physicsType = type;
}

ePhysicsBody BaseObject::getPhysicsBodyType()
{
	return _physicsType;
}
