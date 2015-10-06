
#include "Map.h"
Map::Map()
{
	_mapIndex = NULL;
}

Map::~Map()
{
}

void Map::release()
{
	if (_mapIndex != NULL)
	{
		delete[_width * _height] _mapIndex;
		_mapIndex = NULL;
	}
	_sprite->release();
	delete _sprite;
}
void Map::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (viewport == NULL)
		throw new exception("viewport not accept null value\n");
	int lenght = _width * _height;
	int widthBound = _framewidth * _sprite->getScale().x;
	int heightBound = _frameheight ;
	RECT screenRectEx
	{
		viewport->getPositionWorld().x,
		viewport->getPositionWorld().y,
		viewport->getPositionWorld().x + viewport->getWidth(),
		viewport->getPositionWorld().y - viewport->getHeight() 
	};	// top lớn hơn bottom (sử dụng hệ toạ độ đề các)

	int iBegin = max(screenRectEx.left / widthBound, 0);
	int iEnd = min(screenRectEx.right / widthBound + 1, _width);
	int jBegin = _height - min(screenRectEx.top / heightBound, _height);
	int jEnd = _height - max(screenRectEx.bottom / heightBound + 1, 0);

	// uncomment this block to view How viewport work with tile map => TO KNOW IT IS NOT ONLY A PICTURE :))
	/*
	int iBegin = max(screenRectEx.left / widthBound, 0) + 1;
	int iEnd = min(screenRectEx.right / widthBound, _width) ;
	int jBegin = _height - min(screenRectEx.top / heightBound, _height) + 1;
	int jEnd =_height - max(screenRectEx.bottom / widthBound, 0);
	*/

	GVector2 pos;

	for (int i = iBegin; i < iEnd; i++)
	{
		for (int j = jBegin; j < jEnd; j++)
		{
			_sprite->setIndex(_mapIndex[j * _width + i] - 1);
			pos.x =	i * _framewidth;
			pos.y = (_height - j) * _frameheight;						// nếu có viewport 
			//pos.y = (i / _width) * (_frameheight - 1);				// không có viewport
			_sprite->setPosition(pos);
			_sprite->render(spriteHandle, viewport);
		}
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