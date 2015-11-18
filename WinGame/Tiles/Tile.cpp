#include "Tile.h"

Tile::Tile(Sprite *refSprite, RECT srcRect, int id)
{
	this->_id = id;
	this->_srcRect = srcRect;
	this->_refSprite = refSprite;

}

void Tile::draw(LPD3DXSPRITE spriteHandle, GVector2 position,  Viewport *viewport )
{
	if (_refSprite == NULL)
	{
		throw new exception("refSprite references to not set an instance of object");
	}

	_refSprite->setPosition(position);
	_refSprite->setFrameRect(this->_srcRect);
	//_refSprite->setIndex(_id - 1);
	if (viewport == NULL)
	{
		_refSprite->render(spriteHandle);
	}
	else
	{
		_refSprite->render(spriteHandle, viewport);
	}
}

const int& Tile::getId() const
{
	return this->_id;
}
