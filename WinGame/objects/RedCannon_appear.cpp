#include "RedCannon_appear.h"


RedCannon_appear::RedCannon_appear(int type,GVector2 position) :BaseObject(eID::RED_CANNON_APPEAR)
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::RED_CANNON_APPEAR);
	this->setScale(SCALE_FACTOR);
	this->_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	this->_sprite->setPosition(position);
	_type = type;
}
void RedCannon_appear::init()
{
	

	this->_animation = new Animation(this->_sprite, REDCANNON_APPEAR_SPEED);
	if (this->_type == 1)
	{
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_06"));
	}
	else
	{
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_06"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::RED_CANNON_APPEAR, "run_01"));

	}
}
void RedCannon_appear::update(float deltatime)
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
			this->setStatus(eStatus::DESTROY);
		}
	}
}
void RedCannon_appear::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	this->_animation->draw(spriteHandle, viewport);
}
void RedCannon_appear::release()
{
	SAFE_DELETE(this->_sprite);
}
void RedCannon_appear::setPosition(GVector2 position)
{
	this->_sprite->setPosition(position);
}

RedCannon_appear::~RedCannon_appear()
{
}
