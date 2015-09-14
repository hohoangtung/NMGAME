#define _USE_MATH_DEFINES

#include "Sprite.h"
#include <math.h>
#include "../debug.h"

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

	auto rs = _texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;

	_totalFrames = totalFrames;
	_columns = cols;
	_frameWidth = _texture.getWidth() / cols;
	_frameHeight = _texture.getHeight() * cols / totalFrames;
	_index = 0;
	_currentFrame = GVector2(0, 0);
	
	this->setIndex(0);
	this->updateBounding();
}

void Sprite::release()
{
	this->_texture.release();
}
void Sprite::render(LPD3DXSPRITE spriteHandle)
{
	_texture.render(
		spriteHandle,
		&_frameRect,
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
		&_frameRect,
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
	this->updateBounding();
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

void Sprite::setRotate(float degree)
{
	if (degree == _rotate)
		return;

	_rotate = degree;
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

RECT Sprite::getBounding()
{
	return _bound;
}

void Sprite::setFrameRect(RECT rect)
{
	_frameRect = rect;
}

void Sprite::setFrameRect(float top, float right, float bottom, float left)
{
	_frameRect.top = top;
	_frameRect.right = right;
	_frameRect.left = left;
	_frameRect.bottom = bottom;
}

void Sprite::setFrameRect(float x, float y, int width, int height)
{
	_frameRect.top = y;
	_frameRect.right = x + width;
	_frameRect.left = x;
	_frameRect.bottom = y + height;
}

RECT Sprite::getFrameRect()
{
	return _frameRect;
}

void Sprite::nextFrame()
{
	if (_totalFrames <= 1)
		return;

	this->setIndex(_index + 1);
}

void Sprite::setIndex(int index)
{
	if(_index != index)
		_index = index;

	this->setCurrentFrame();
}

void Sprite::setFrameRect()
{
	this->_frameRect.left = (long)_currentFrame.x * _frameWidth;
	this->_frameRect.right = _frameRect.left + _frameWidth;
	this->_frameRect.top = (long)_currentFrame.y * _frameHeight;
	this->_frameRect.bottom = _frameRect.top + _frameHeight;
}

void Sprite::setCurrentFrame()
{
	if (_index >= _totalFrames)
		_index = _index % _totalFrames;

	__debugoutput(_index);
	
	this->_currentFrame.x = _index % _columns;
	this->_currentFrame.y = _index / _columns;

	this->setFrameRect();
}

void Sprite::updateBounding()
{
	float scaleW = _frameWidth * _scale.x;
	float scaleH = _frameHeight * _scale.y;

	this->_bound.left = _position.x - scaleW * _origin.x;
	this->_bound.bottom = _position.y - scaleH * _origin.y;
	this->_bound.right = _bound.left + scaleW;
	this->_bound.top = _bound.bottom + scaleH;

	//rotate...
}
