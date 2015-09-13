#include "Sprite.h"

US_FRAMEWORK

Sprite::~Sprite()
{
	
}

Sprite::Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int totalFrames, int cols)
{
	_origin = GVector2(0.5f, 0.5f);
	_scale = GVector2(1.0f, 1.0f);
	_zIndex = 1;
	_rotate = 0.0f;
	_velocity = GVector2(0, 0);
	_accelerate = GVector2(0, 0);

	auto rs = _texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;

	float w = _texture.getWidth() / cols;
	float h = _texture.getHeight() * cols / totalFrames;

	_animation = new Animation(totalFrames, cols, w, h);
}

void Sprite::release()
{
	this->_texture.release();

	SAFE_DELETE(_animation);
}
void Sprite::render(LPD3DXSPRITE spriteHandle)
{
	_texture.render(
		spriteHandle,
		&_animation->getFrameRect(),
		_position,
		_scale,
		_rotate,
		_origin,
		_zIndex
		);

	return;
}

void Sprite::render(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_texture.render(
		spriteHandle,
		&_animation->getFrameRect(),
		*viewport,
		_position,
		_scale,
		_rotate,
		_origin,
		_zIndex
		);
}

void Sprite::setPosition(int x, int y, int z)
{
	GVector3 v(x, y, z);
	this->setPosition(v);
}

void Sprite::setPosition(GVector3 vector)
{
	this->_position = GVector2(vector.x, vector.y);
	this->updateBounding();
}

void Sprite::setPosition(GVector2 position)
{
	this->_position = GVector2(position.x, position.y);
}

void Sprite::setPositionX(float x)
{
	if (x != _position.x)
		_position.x = x;

	this->updateBounding();
}

void Sprite::setPositionY(float y)
{
	if (y != _position.y)
		_position.y = y;

	this->updateBounding();
}

GVector2 Sprite::getScale()
{
	return _scale;
}

void Sprite::setScale(GVector2 scale)
{
	if (scale == _scale)
		return;

	_scale = scale;
}

void Sprite::setScale(float scale)
{
	if (scale != _scale.x || scale != _scale.y)
	{
		_scale.x = scale;
		_scale.y = scale;
	}
}

void Sprite::setScaleX(float sx)
{
	if (sx != _scale.x)
		_scale.x = sx;
}

void Sprite::setScaleY(float sy)
{
	if (sy != _scale.y)
		_scale.y = sy;
}

float Sprite::getRotate()
{
	return _rotate;
}

void Sprite::setRotate(float degree)
{
	if (degree == _rotate)
		return;

	_rotate = degree;
}

GVector2 Sprite::getOrigin()
{
	return _origin;
}

void Sprite::setOrigin(GVector2 origin)
{
	if (origin != _origin)
		_origin = origin;
}

void Sprite::setZIndex(int z)
{
	if (z != _zIndex)
		_zIndex = z;
}

int Sprite::getZIndex()
{
	return _zIndex;
}

void Sprite::update(float dt)
{
	_velocity += _accelerate * dt / 1000;
	_position += _velocity * dt / 1000;

	//update animation
	_animation->update(dt);
}

RECT Sprite::getBounding()
{
	return _bound;
}

void Sprite::setVelocity(GVector2 vel)
{
	if (_velocity != vel)
		_velocity = vel;
}

void Sprite::setVelocity(float x, float y)
{
	if (_velocity.x != x || _velocity.y != y)
	{
		_velocity.x = x;
		_velocity.y = y;
	}
}

void Sprite::setVelocityX(float velX)
{
	if (_velocity.x != velX)
		_velocity.x = velX;
}

void Sprite::setVelocityY(float velY)
{
	if (_velocity.y != velY)
		_velocity.y = velY;
}

void Sprite::setAccelerate(GVector2 acc)
{
	if (_accelerate != acc)
		_accelerate = acc;
}

void Sprite::setAccelerate(float x, float y)
{
	if (_accelerate.x != x || _accelerate.y != y)
	{
		_accelerate.x = x;
		_accelerate.y = y;
	}
}

void Sprite::setAccelerateX(float accX)
{
	if (_accelerate.x != accX)
		_accelerate.x = accX;
}

void Sprite::setAccelerateY(float accY)
{
	if (_accelerate.y != accY)
		_accelerate.y = accY;
}

Animation * Sprite::getAnimation()
{
	return _animation;
}

void Sprite::updateBounding()
{
	this->_bound.left = _position.x - _animation->getFrameWidth() * _origin.x;
	this->_bound.right = _bound.left + _animation->getFrameWidth();
	this->_bound.top = _position.y + _animation->getFrameHeight() * _origin.y;
	this->_bound.bottom = _bound.right + _animation->getFrameHeight();
}

GVector2 Sprite::getPosition()
{
	return _position;
}

float Sprite::getPositionX()
{
	return _position.x;
}

float Sprite::getPositionY()
{
	return _position.y;
}
