
#include "IComponent.h"
#include "..\debug.h"
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
	this->_velocity +=this->_accelerate * deltatime / 1000;
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
		this->_additionalVeloc = this->_gravity * deltatime / 1000;
		break;
	case SHALLOWED:
		this->_additionalVeloc = VECTOR2ZERO;
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
void Gravity::setGravity(GVector2 gravity)
{
	this->_gravity = gravity;
}
Gravity::Gravity(GVector2 gravity, Movement *movement)
{
	this->_gravity = gravity;
	this->_refmovement = movement;
	this->_status = FALLING__DOWN;
}
#pragma endregion

#pragma region SinMovement
SinMovement::SinMovement(GVector2 amplitude, float frequency, Sprite* refsprite)
{
	this->_amplitude = amplitude;
	this->_refSprite = refsprite;
	_radianVeloc = frequency * 2 * M_PI;
	_radian = 0.0f;
	this->_linearVeloc = _amplitude * _radianVeloc; //( A * ω)
}
void SinMovement::update(float deltatime)
{
	/*
		góc xoay được tính theo công thức
		φ = ω * t
	*/
	_radian += _radianVeloc * deltatime / 1000;

	/*
		vận tốc tuyến tính được tính theo công thức
		v = -A * ω * sin(ωt)
		(_linearVeloc = A * ω)
	*/
	auto veloc = -_linearVeloc * sin(_radian);

	auto pos = this->_refSprite->getPosition();
	pos += veloc * deltatime / 1000;
	this->_refSprite->setPosition(pos);
}

void SinMovement::setAmplitude(GVector2 amplitude)
{
	this->_amplitude = amplitude;
	this->_linearVeloc = _amplitude * _radianVeloc;
}
void SinMovement::setFrequency(float freq)
{
	_radianVeloc = freq;
	this->_linearVeloc = _amplitude * _radianVeloc;
}
#pragma endregion
