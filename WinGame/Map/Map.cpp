
#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
}
int i = 0;
void Map::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	int lenght = _width * _height;
	RECT screenRect
	{
		viewport->getPositionWorld().x,
		viewport->getPositionWorld().y,
		viewport->getPositionWorld().x + viewport->getWidth(),
		viewport->getPositionWorld().y - viewport->getHeight()
	};
	RECT bound;
	for (int i = 0; i < lenght; i++)
	{
		_sprite->setIndex(_mapIndex[i] - 1);
		
		GVector2 pos;
		pos.x = (i % _width) * ( _framewidth - 0);
		pos.y = (_height - (i / _width)) * (_frameheight - 0);		// nếu có viewport 
		//pos.y = (i / _width) * (_frameheight - 1);				// không có viewport

		_sprite->setPosition(pos);
		bound = _sprite->getBounding();
		if (bound.right < screenRect.left)
			continue;
		if (bound.bottom > screenRect.top)
			continue;
		if (bound.left > screenRect.right)
			continue;
		if (bound.top < screenRect.bottom)
			continue;
		_sprite->render(spriteHandle, viewport);

	}
}

void Map::init(string filepath)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::MAPSTAGE1);
	_sprite->setZIndex(0.0f);
	_sprite->setOrigin(VECTOR2ZERO);
	this->_framewidth = _sprite->getFrameWidth();
	this->_frameheight = _sprite->getFrameHeight();

	FILE* file;
	file = fopen(filepath.c_str(), "r");
	
	if (file == NULL)
		return;
	fscanf(file, "%d", &_width);
	fscanf(file, "%d", &_height);
	_mapIndex = new int[_width * _height];
	int i = 0;
	while (!feof(file))
	{
		fscanf(file, "%d", &_mapIndex[i++]);
	}
	fclose(file);


}