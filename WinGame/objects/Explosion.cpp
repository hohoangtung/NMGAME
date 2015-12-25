

#include "Explosion.h"
/*
	Explosion dùng để tạo vụ nổ khi nhân vật hoặc đối tượng chết.
	hiện tại có 2 kiểu nổ. (xem hình resource)
	lớp nào chứa explosion nhớ quản lý huỷ đối tương này
*/
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
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_04"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_03"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_01"));
		this->_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::EXPLOSION, "type2_02"));
	}
}

void Explosion::update(float deltatime)
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->update(deltatime);
	if (this->_type == 1 && this->_animation->getIndex() == 5)
	{
		this->setStatus(eStatus::DESTROY);
	}
	
	if (this->_type == 2 && this->_animation->getIndex() == 9)
	{	
		this->setStatus(eStatus::DESTROY);
	}
}

void Explosion::setPosition(GVector2 position)
{
	this->_sprite->setPosition(position);
}

void Explosion::setTimeAnimated(float time)
{
	this->_animation->setTimeAnimate(time);
}

void Explosion::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	this->_animation->draw(spriteHandle, viewport);
}
void Explosion::release()
{
	SAFE_DELETE(this->_sprite);
	SAFE_DELETE(_animation);
}
