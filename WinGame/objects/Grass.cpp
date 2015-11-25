

#include "Grass.h"


Grass::Grass(GVector2 pos) : BaseObject(eID::GRASS)
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::GRASS);
	this->_sprite->setPosition(pos);
	this->_sprite->setScale(SCALE_FACTOR);
}
 
Grass::~Grass()
{
}

void Grass::init()
{
	auto collisionBody = new CollisionBody(this);
	
	_componentList["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::TOP);
}

void Grass::update(float deltatime)
{
	for (auto component : _componentList)
	{
		component.second->update(deltatime);
	}
}

void Grass::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	this->_sprite->render(spriteHandle, viewport);
}

void Grass::release()
{
	for (auto component : _componentList)
	{
		delete component.second;
	}
	_componentList.clear();
}