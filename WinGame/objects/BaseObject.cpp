
#include "BaseObject.h"
BaseObject::BaseObject(eID id)
{
	_sprite = SpriteManager::getInstance()->getSprite(id);
	
}

BaseObject::~BaseObject()
{
}