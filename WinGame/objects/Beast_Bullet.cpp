#include "Beast_Bullet.h"

void BeastBullet::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_animation->update(deltatime);
	Bullet::update(deltatime);
}
void BeastBullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_animation->draw(spriteHandle, viewport);
}
void BeastBullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);

	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "beastbullet1"));
	_sprite->setZIndex(1.0f);
	_animation = new Animation(_sprite, 0.07);
	_animation->addFrameRect(eID::BULLET, "beastbullet1", "beastbullet2", "beastbullet3", "beastbullet4", NULL);

	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	GVector2 veloc = Bullet::initveloc(BEAST_BULLET_SPEED);

	auto movement = new Movement(GVector2(0, 0), veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));


	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);

	_bursttime = new StopWatch();
}
BeastBullet::BeastBullet(GVector2 startposition, float degree) : Bullet(startposition, eBulletType(BEASTBULLET | ENEMY_BULLET), degree)
{
}
RECT BeastBullet::getBounding()
{
	auto basebound = BaseObject::getBounding();
	basebound.left += 2;
	basebound.right-= 2;
	basebound.top -= 2;
	basebound.bottom += 2;
	return basebound;
}
BeastBullet::~BeastBullet()
{
}