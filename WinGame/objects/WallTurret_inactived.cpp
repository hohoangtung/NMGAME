#include "WallTurret_inactived.h"


WallTurret_inactived::WallTurret_inactived(int type, GVector2 pos) :BaseObject(eID::WALL_TURRET_INACTIVED)
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET_INACTIVED);
	this->setScale(SCALE_FACTOR);
	this->_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	this->_sprite->setPosition(pos);
	_type = type;
}
WallTurret_inactived::WallTurret_inactived(int type,float x, float y) : BaseObject(eID::WALL_TURRET_INACTIVED)
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET_INACTIVED);
	this->setScale(SCALE_FACTOR);
	this->_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	GVector2 pos(x, y);
	this->setPosition(pos);
	_type = type;
}

void WallTurret_inactived::init()
{
	if (_type == 1)
	{

		this->_animation = new Animation(this->_sprite, WALLTURRET_INACTIVED_SPEED);
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_06"));
	}
	else if(_type==2)
	{
		this->_animation = new Animation(this->_sprite, WALLTURRET_INACTIVED_SPEED);
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_06"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::WALL_TURRET_INACTIVED, "run_01"));
	}
}
void WallTurret_inactived::update(float deltatime)
{
	
	this->_animation->update(deltatime);
	if (_type == 1)
	{

		if (this->_animation->getIndex() == 5)
		{
			this->setStatus(eStatus::WAITING);
		}
	}
	if (_type == 2)
	{
		if (this->_animation->getIndex() == 0)
		{
			this->setStatus(eStatus::WAITING);
		}
	}
}
void WallTurret_inactived::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->draw(spritehandle, viewport);

}
void WallTurret_inactived::setPosition(GVector2 position)
{
	this->_sprite->setPosition(position);
}
IComponent* WallTurret_inactived::getComponent(string componentname)
{
	return _listComponent.find(componentname)->second;
}
void WallTurret_inactived::release()
{
	SAFE_DELETE(this->_sprite);
}
WallTurret_inactived::~WallTurret_inactived()
{
}
