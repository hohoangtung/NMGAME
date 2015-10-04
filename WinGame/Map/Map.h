
#ifndef __MAP_H__
#define __MAP_H__
#include <string>
#include <vector>
#include "..\FrameWork\define.h"
#include "..\FrameWork\SpriteManager.h"
using namespace std;
US_FRAMEWORK

class Map
{
public:
	void init(string filepath);
	void draw(LPD3DXSPRITE);
	Map();
	~Map();

private:
	int* _mapIndex;
	int _width;
	int _height;
	Sprite* _sprite;
};


#endif // !__MAP_H__
