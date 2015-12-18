#include "TextSprite.h"


void TextSprite::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(_type);
	this->rows = _sprite->getTextureHeight() / _sprite->getFrameHeight();
	this->_columns = _sprite->getTextureWidth() / _sprite->getFrameWidth();
	_framewidth = _sprite->getFrameWidth();
}
void TextSprite::update(float deltatime)
{

}

void TextSprite::draw(LPD3DXSPRITE spriteHandle)
{
	this->draw(spriteHandle, nullptr);
}
void TextSprite::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	int x = 0, y = 0;
	int index = 0;
	int i = 0;
	for each (char c in _text)
	{
		//x = ((int)c) % _columns;
		//y = ((int)c) / _columns;
		if (c >= 'A' && c <= 'Z')
		{
			index = ((int)c) - 'A';
		}
		if (c >= 'a' && c <= 'z')
		{
			index = ((int)c) - 'a';
		}
		if (c >= '0' && c <= '9')
		{
			index = (((int)c) - '0') + 26;
		}
		if (c == ' ')
		{
			index = 53;
		}
		_sprite->setIndex(index);
		_sprite->setPosition(GVector2(_position.x + _sprite->getFrameWidth() * i, _position.y));
		if (viewport == nullptr)
		{
			_sprite->render(spriteHandle);
		}
		else
		{
			_sprite->render(spriteHandle, viewport);
		}
		i++;
	}
}
void TextSprite::release()
{

}
TextSprite::TextSprite(eID type, string text, GVector2 position) : BaseObject(type)
{
	this->_type = type;
	this->_text = text;
	this->_position = position;
}

TextSprite::~TextSprite()
{
}
