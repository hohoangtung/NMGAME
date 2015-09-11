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
	//Sprite* sp = new Sprite(spriteHandle, filePath, countFrame, FrameperRow)
	// this->_listSprite.add(pair(eID::OBJECT, sp));

}
Sprite* SpriteManager::getSprite(eID id)
{
	//return new sprite with copy constructor.
	// not return reference.
}
void SpriteManager::releaseSprite(eID id)
{
	//remove sprite from memorry
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
