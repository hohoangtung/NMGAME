#include "ScubaSoldier.h"

ScubaSoldier::ScubaSoldier(GVector2 pos) : BaseEnemy(eID::SCUBASOLDIER)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SCUBASOLDIER);
	_sprite->setFrameRect(0, 0, 15, 30);
	this->setPosition(pos);
}

ScubaSoldier::ScubaSoldier(float x, float y) : BaseEnemy(eID::SCUBASOLDIER)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::RIFLEMAN);
	_sprite->setFrameRect(0, 0, 15, 30);
	this->setPosition(GVector2(x, y));
}

ScubaSoldier::~ScubaSoldier() {};

void ScubaSoldier::init()
{
	this->setScale(SCALE_FACTOR);

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Rifleman::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Rifleman::onCollisionEnd);

	_animations[HIDDEN] = new Animation(_sprite, SCUBASOLDIER_ANIMATION_SPEED);
	_animations[HIDDEN]->addFrameRect(eID::SCUBASOLDIER, "hidden", NULL);

	_animations[SHOOTING] = new Animation(_sprite, SCUBASOLDIER_ANIMATION_SPEED);
	_animations[SHOOTING]->addFrameRect(eID::SCUBASOLDIER, "shooting", NULL);

	this->setHitpoint(SCUBASOLDIER_HITPOINT);
	this->setScore(SCUBASOLDIER_SCORE);

 	this->setStatus(eStatus::SHOOTING);
}

void ScubaSoldier::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::WAITING || this->getStatus() == eStatus::BURST)
		return;
	_animations[this->getStatus()]->draw(spritehandle, viewport);
}

void ScubaSoldier::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	if (this->_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(this->_explosion);
	SAFE_DELETE(this->_sprite);
}

void ScubaSoldier::update(float deltatime)
{
	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->getStatus() == eStatus::BURST)
	{
		if (_explosion == nullptr)
		{
			auto pos = this->getPosition();
			_explosion = new Explosion(1);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(pos);
		}
		else if (_explosion->getStatus() == eStatus::DESTROY)
		{
			this->setStatus(eStatus::DESTROY);
		}
		return;
	}
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::WAITING)
		return;
	if (this->getHitpoint() <= 0)
	{
		this->setStatus(eStatus::BURST);
		return;
	}
}

void ScubaSoldier::onCollisionBegin(CollisionEventArg* collision_event) {}
void ScubaSoldier::onCollisionEnd(CollisionEventArg* collision_vent) {}

float ScubaSoldier::checkCollision(BaseObject *object, float dt)
{
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	collisionBody->checkCollision(object, dt);
	return 0.0f;
}



