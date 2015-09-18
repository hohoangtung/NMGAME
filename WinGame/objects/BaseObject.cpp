
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
		this->_status = status;
}

void BaseObject::setPosition(GVector3 vector)
{
	Transformable::setPosition(vector);
	this->updateTransform();
}

void BaseObject::setPosition(float x, float y, float z)
{
	Transformable::setPosition(x, y, z);

	this->updateTransform();
}

void BaseObject::setPosition(GVector2 position)
{
	Transformable::setPosition(position);

	this->updateTransform();
}

void BaseObject::setPosition(float x, float y)
{
	Transformable::setPosition(x, y);

	this->updateTransform();
}

void BaseObject::setPositionX(float x)
{
	Transformable::setPositionX(x);

	this->updateTransform();
}

void BaseObject::setPositionY(float y)
{
	Transformable::setPositionY(y);

	this->updateTransform();
}

void BaseObject::updateTransform()
{
	_sprite->setOrigin(_origin);
	_sprite->setPosition(_position);
	_sprite->setScale(_scale);
	_sprite->setRotate(_rotate);
}

Sprite * BaseObject::getSprite()
{
	return _sprite;
}
