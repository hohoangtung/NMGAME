

#include "Explosion.h"
Explosion::Explosion(int type) : BaseObject(eID::EXPLOSION)
{
	this->_type = type;
}

Explosion::~Explosion()
{
}


void Explosion::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::EXPLOSION);

	this->_animation = new Animation(this->_sprite, SPEED_ANIMATION);
	if (this->_type == 1)
	{
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type1_06"));
	}
	else
	{
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_02"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_05"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_06"));

	}
}

void Explosion::update(float deltatime)
{
	this->_animation->update(deltatime);
	if (this->_animation->getIndex() == 5)
	{
		this->setStatus(eStatus::DESTROY);
	}
}

void Explosion::setPosition(GVector2 position)
{
	this->_sprite->setPosition(position);
}

void Explosion::draw(LPD3DXSPRITE spriteHandle)
{
	this->_animation->draw(spriteHandle, PlayScene::getViewport());
}
void Explosion::release()
{
	SAFE_DELETE(this->_sprite);
}
