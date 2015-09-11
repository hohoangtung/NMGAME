
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

	this->_count = count;
	this->_spriteperrow = SPR;
	this->_rowCount = count / SPR;

	this->_position = D3DXVECTOR3(0, 0, 1);
	this->setIndex(0);
	
	auto rs = this->_Texture.loadFromFile(spriteHandle, filePath);
	if (rs != D3D_OK)
		throw;

	this->_frameWidth = _Texture.getWidth() / _spriteperrow;
	this->_frameHeight = _Texture.getHeight()  / _rowCount;

	this->setFrameRect();
}

void Sprite::render(LPD3DXSPRITE spriteHandle)
{
	/// view port and transform to decac vector

	//D3DXMATRIX mt;
	//D3DXMatrixIdentity(&mt);	// ma trận đơn vị
	//mt._22 = -1.0f;
	//mt._41 = 400;
	//mt._42 = 400;
	//D3DXVECTOR4 vp_pos;
	//D3DXVec3Transform(&vp_pos, &_position, &mt);

	//D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);
	//_Texture.render(
	//	spriteHandle,
	//	&_frameRect,
	//	NULL,
	//	&p);
	//D3DXVECTOR3 p(vp_pos.x, vp_pos.y, 0);

	// normal render without viewport
	_Texture.render(
		spriteHandle,
		&_frameRect,
		NULL,
		&_position);
	return;
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
	this->_position = vector;

	this->_bound.left = _position.x;   // dựa vào hệ toạ độ top left làm gốc. Chưa biết hệ DECAC có đúng hay không
	this->_bound.top = _position.y;
	this->_bound.right = _bound.left + this->_frameWidth;
	this->_bound.bottom = _bound.right + this->_frameHeight;
}

void Sprite::next()
{
	this->setIndex(this->_index + 1);
}

GVector3 Sprite::getPosition()
{
	return _position;
}
