
#include "IComponent.h"

#pragma region Movement
Movement::Movement(GVector2 accel, GVector2 veloc, Sprite* refSprite)
{
	this->_accelerate = accel;
	this->_velocity = veloc;
	this->_refSprite = refSprite;
}
void Movement::update(float deltatime)
{
	if (_refSprite == NULL)
		return;
	auto position = this->_refSprite->getPosition();
	this->_velocity += this->_accelerate * deltatime / 1000;
	position += this->_velocity * deltatime / 1000;
	this->_refSprite->setPosition(position);
}
void Movement::setAccelerate(GVector2 accel)
{
	this->_accelerate = accel;
}
void Movement::setVelocity(GVector2 veloc)
{
	this->_velocity = veloc;
}
GVector2 Movement::getAccelerate()
{
	return this->_accelerate;
}
GVector2 Movement::getVelocity()
{
	return this->_velocity;
}
#pragma endregion

#pragma region Gravity
void Gravity::update(float deltatime)
{
	switch (_status)
	{
	case FALLING__DOWN:
		this->_additionalVeloc += this->_gravity * deltatime / 1000;
		break;
	case SHALLOWED:
		this->_additionalVeloc = GVector2(0.0f, 0.0f);
	default:
		break;
	}
	auto veloc = this->_refmovement->getVelocity();
	this->_refmovement->setVelocity(veloc + _additionalVeloc);
}

void Gravity::setStatus(eGravityStatus status)
{
	this->_status = status;
}

Gravity::Gravity(GVector2 gravity, Movement *movement)
{
	this->_gravity = gravity;
	this->_refmovement = movement;
	this->_status = FALLING__DOWN;
}
#pragma endregion
