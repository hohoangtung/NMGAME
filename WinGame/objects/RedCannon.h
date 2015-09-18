
#ifndef __REDCANNON_H__
#define __REDCANNON_H__

#include "BaseObject.h"
class RedCannon : public BaseObject
{
public:
	RedCannon(GVector2 position) : BaseObject(eID::REDCANNON)
	{
		this->_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
		this->_sprite->setPosition(position);
	}
	~RedCannon(){}
	void init()
	{
	
	}
	void update(float deltatime) 
	{
		this->_sprite->nextFrame();
	}
	void draw(LPD3DXSPRITE spriteHandle)
	{
		this->_sprite->render(spriteHandle);
	}
	void release() {}
private:

};

#endif // !__REDCANNON_H__
