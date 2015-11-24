#include "Bullet.h"
Bullet::Bullet(GVector2 startPosition, eDirection dir) : BaseObject(eID::BULLET)
{
	_startPosition = startPosition;
	_direction = dir;
}

Bullet::Bullet(GVector2 startPosition, float degree) : BaseObject(eID::BULLET)
{
	_startPosition = startPosition;
	_direction = eDirection::NONE;
	_degree = degree;
}

Bullet::~Bullet()
{
}

void Bullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "normal_bullet"));

	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	GVector2 veloc;

	if (_direction != eDirection::NONE)
	{
		if ((_direction & eDirection::LEFT) == eDirection::LEFT)
		{
			veloc.x = -NORMAL_BULLET_SPEED;
		}
		else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		{
			veloc.x = NORMAL_BULLET_SPEED;
		}
		else
		{
			veloc.x = 0;
		}

		if ((_direction & eDirection::TOP) == eDirection::TOP)
		{
			veloc.y = NORMAL_BULLET_SPEED;
		}
		else if ((_direction & eDirection::BOTTOM) == eDirection::BOTTOM)
		{
			veloc.y = -NORMAL_BULLET_SPEED;
		}
		else
		{
			veloc.y = 0;
		}
	}
	else
	{
		auto rad = _degree * M_PI / 180;
		veloc.x = sin(rad) * NORMAL_BULLET_SPEED;
		veloc.y = cos(rad) * NORMAL_BULLET_SPEED;
	}

	auto movement = new Movement(GVector2(0, 0), veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement",movement));
	//_componentList["Movement"] = movement;

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody",collisionBody));
	//_componentList.find("CollisionBody")->second = collisionBody;
	//_componentList["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

void Bullet::update(float deltatime)
{
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Bullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	_sprite->render(spriteHandle, viewport);
}

void Bullet::release()
{
}

int Bullet::getDamage()
{
	return _damage;
}

GVector2 Bullet::getVelocity()
{
	auto move = (Movement*)this->_componentList.find("Movement")->second;
	//auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bullet::onCollisionBegin(CollisionEventArg* collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();
	switch (objectID)
	{
	case AIRCRAFT:
		collision_arg->_otherObject->setStatus(eStatus::BURST);
		break;
	case BOX:	
		OutputDebugString(L"hit...\n");
		break;
	case SOLDIER: case RIFLEMAN:
		((BaseEnemy*)collision_arg->_otherObject)->dropHitpoint();
		break;
	}
}

float Bullet::checkCollision(BaseObject * object, float dt)
{
	auto body = (CollisionBody*)_componentList.find("CollisionBody")->second;
	//auto body = (CollisionBody*)_componentList["CollisionBody"];
	body->checkCollision(object, dt);

	return 0.0f;
}
