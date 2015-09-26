
#include "Map.h"



Map::Map()
{
}

Map::~Map()
{
}

void Map::draw(LPD3DXSPRITE spriteHandle)
{
	int lenght = _width * _height;
	for (int i = 0; i < lenght; i++)
	{
		_sprite->setIndex(_mapIndex[i]);
		GVector2 pos;
		pos.x = i % _width * 16 + 200;
		pos.y = i / _width * 16 + 200;

		_sprite->setPosition(pos);
		_sprite->render(spriteHandle);
	}
}

void Map::init(string filepath)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::MAPSTAGE1);
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