#include "SpriteManager.h"
US_FRAMEWORK

SpriteManager* SpriteManager::_instance = nullptr;
SpriteManager::SpriteManager(void)
{
	
}

SpriteManager::~SpriteManager(void)
{
	if (_listSprite.empty() == false)
		_listSprite.clear();
}

void SpriteManager::loadResource(LPD3DXSPRITE spriteHandle)
{
	Sprite* sp = new Sprite(spriteHandle, L"Flower.png", 4, 4);
	this->_listSprite.insert(pair<eID, Sprite>(eID::FLOWER, *sp));
}
Sprite* SpriteManager::getSprite(eID id)
{
	Sprite it = this->_listSprite.find(id)->second;
	return new Sprite(it);
}
void SpriteManager::releaseSprite(eID id)
{
	this->_listSprite.erase(id);
}
SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}

void SpriteManager::release()
{
	delete _instance;
	_instance = nullptr;
}
