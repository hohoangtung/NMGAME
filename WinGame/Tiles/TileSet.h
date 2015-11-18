#ifndef __TILESET__
#define __TILESET__

#include <string>
#include <vector>
#include <stdexcept>

#include "config.h"
#include "Tile.h"

// Class helper for class Map.
class TileSet 
{
private:
	Sprite *_tileImage;
	int _widthtile;  // kích thước tile
	int _heighttile; // kích thước tile
	std::string _filename;
	int _columns;
	int _rows;
	std::vector<Tile*> _listTiles;

public:
	TileSet(eID spriteId);

	int getWidthtile() const;
	void setWidthtile(const int &value);
	int getHeighttile() const;
	void setHeighttile(const int &value);

	void loadListTiles(pugi::xml_node& node);
	Sprite* getSprite();
	void draw(LPD3DXSPRITE spriteHandle, int id, GVector2 position,  Viewport *viewport );
};


#endif	//#ifndef __TILESET__
