#include "TileSet.h"
#include "Tile.h"

TileSet::TileSet(eID spriteId)
{
	Sprite* sp= SpriteManager::getInstance()->getSprite(spriteId);
	this->_tileImage = sp;
	this->_heighttile = sp->getFrameHeight();
	this->_widthtile = sp->getFrameWidth();
		
}

void TileSet::draw(LPD3DXSPRITE spriteHandle, int id, GVector2 position,  Viewport *viewport )
{
	for(auto tile : _listTiles)
	{
		if (tile->getId() == id)
		{
			tile->draw(spriteHandle, position, viewport);
			return;
		}
		else
		{
			continue;
		}
	}
}

void TileSet::loadListTiles(pugi::xml_node& node)
{
	/*
		tilelistnode chứa tất cả các element <Tile> trong file xml.
		Cấu trúc dạng:
		<Tiles>
			<Tile>
		<Tiles>
	*/
	auto tilelistnode = node.child("Tiles").children();
	Tile*  tile = nullptr;
	RECT srcRECT = { 0,0,0,0};
	int id = 0;
	for each (pugi::xml_node_iterator it in tilelistnode)
	{
		id = it->attribute("Id").as_int();
		srcRECT.top = it->child("Rect").attribute("Y").as_int();
		srcRECT.left = it->child("Rect").attribute("X").as_int();
		srcRECT.bottom = srcRECT.top + it->child("Rect").attribute("Width").as_int() + 1; 
		srcRECT.right = srcRECT.left + it->child("Rect").attribute("Height").as_int() + 1;
		this->_listTiles.push_back(new Tile(this->_tileImage, srcRECT, id));
	}
}

int TileSet::getWidthtile() const
{
	return _widthtile;
}

void TileSet::setWidthtile(const int &value)
{
	this->_widthtile = value;
}

int TileSet::getHeighttile() const
{
	return this->_heighttile;
}

void TileSet::setHeighttile(const int &value)
{
	this->_heighttile = value;
}

Sprite* TileSet::getSprite()
{
	return this->_tileImage;
}
