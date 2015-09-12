
#include "Sprite.h"
US_FRAMEWORK


Sprite::~Sprite()
{
	/*
		_Texture.~Texture();
	*/
}
Sprite::Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int count, int SPR)
{
	_origin = GVector2(0.5f, 0.5f);
	_scale = GVector2(1.0f, 1.0f);

	this->_count = count;
	this->_spriteperrow = SPR;
	this->_rowCount = count / SPR;

	this->_position = GVector2(0, 0);
	this->setIndex(0);
	
	auto rs = this->_Texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;

	this->_frameWidth = _Texture.getWidth() / _spriteperrow;
	this->_frameHeight = _Texture.getHeight()  / _rowCount;

	this->setFrameRect();
}
void Sprite::release()
{
	this->_Texture.release();
}
void Sprite::render(LPD3DXSPRITE spriteHandle)
{
	_Texture.render(
		spriteHandle,
		&_frameRect,
		_position,
		_scale,
		_rotate,
		_origin
		);

	return;
}

void Sprite::render(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_Texture.render(
		spriteHandle,
		&_frameRect,
		*viewport,
		_position,
		_scale,
		_rotate,
		_origin
		);
}

void Sprite::setIndex(int index)
{
	this->_index = index % _count;
	
	this->setCurrentFrame();
}

void Sprite::setCurrentFrame()
{
	if (_index > this->_count)
		_index = _index % _count;
	this->_curFrame.x = _index % _spriteperrow;
	this->_curFrame.y = _index / _spriteperrow;
	this->setFrameRect();
}

void Sprite::setFrameRect()
{
	this->_frameRect.left = (long) _curFrame.x * _frameWidth;
	this->_frameRect.right = _frameRect.left + _frameWidth;
	this->_frameRect.top = (long) _curFrame.y * _frameHeight;
	this->_frameRect.bottom = _frameRect.top + _frameHeight;
}

void Sprite::setPosition(int x, int y, int z)
{
	GVector3 v(x, y, z);
	this->setPosition(v);
}

void Sprite::setPosition(GVector3 vector)
{
	this->_position = GVector2(vector.x, vector.y);

	this->_bound.left = _position.x;   // dựa vào hệ toạ độ top left làm gốc. Chưa biết hệ DECAC có đúng hay không
	this->_bound.top = _position.y;
	this->_bound.right = _bound.left + this->_frameWidth;
	this->_bound.bottom = _bound.right + this->_frameHeight;
}

void Sprite::setPositionX(float x)
{
	if (x != _position.x)
		_position.x = x;
}

void Sprite::setPositionY(float y)
{
	if (y != _position.y)
		_position.y = y;
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

void Sprite::next()
{
	this->setIndex(this->_index + 1);
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
