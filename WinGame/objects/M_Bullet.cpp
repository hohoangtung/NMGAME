#include "M_Bullet.h"

void MBullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "m_bullet"));

	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	GVector2 veloc = Bullet::initveloc(M_BULLET_SPEED);

	auto movement = new Movement(GVector2(0, 0), veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

MBullet::MBullet(GVector2 startPosition, float degree)
	: Bullet(startPosition, eBulletType(BILL_BULLET | M_BULLET), degree)
{

}

MBullet::~MBullet()
{
}
