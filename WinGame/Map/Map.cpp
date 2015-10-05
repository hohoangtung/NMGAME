
#include "Map.h"
#include "..\debug.h"
#include <ddraw.h>

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
	RECT rect;

	for (int i = 0; i < lenght; i++)
	{
		_sprite->setIndex(_mapIndex[i] - 1);
		
		GVector2 pos;
		pos.x = (i % _width) * ( _framewidth - 0);
		pos.y = (_height - (i / _width)) * (_frameheight - 0);		// nếu có viewport 
		//pos.y = (i / _width) * (_frameheight - 1);				// không có viewport

		if (pos.x > WINDOW_WIDTH)
			continue;
		if (pos.y > WINDOW_HEIGHT)
			continue;
		_sprite->setPosition(pos);
		_sprite->render(spriteHandle, viewport);

	}
}

void Map::init(string filepath)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::MAPSTAGE1);
	_sprite->setZIndex(0.0f);

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