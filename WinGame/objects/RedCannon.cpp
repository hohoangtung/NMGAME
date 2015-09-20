
#include"RedCannon.h"

RedCannon::RedCannon(GVector2 position) : BaseObject(eID::REDCANNON)
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	this->_sprite->setPosition(position);
}
RedCannon::~RedCannon()
{

}
void RedCannon::init()
{

}
void RedCannon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	this->_sprite->render(spriteHandle, viewport);
}
void RedCannon::update(float deltatime)
{
	this->_sprite->nextFrame();
}