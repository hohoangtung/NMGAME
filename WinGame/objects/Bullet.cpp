#include "Bullet.h"
#include "../FrameWork/Managers/SoundManager.h"

Bullet::Bullet(GVector2 startPosition, eBulletType type,eDirection dir) : BaseObject(eID::BULLET)
{
	_startPosition = startPosition;
	_direction = dir;
	_type = type;
}

Bullet::Bullet(GVector2 startPosition, eBulletType type,float degree) : BaseObject(eID::BULLET)
{
	_startPosition = startPosition;
	_direction = eDirection::NONE;
	_degree = degree;
	_type = type;
}

eBulletType Bullet::getBulletType()
{
	return _type;
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

	GVector2 veloc = this->initveloc(NORMAL_BULLET_SPEED);

	//// Tung Ho
	//// 28/11 đoạn code sau được đưa vào initveloc
	//if (_direction != eDirection::NONE)
	//{
	//	if ((_direction & eDirection::LEFT) == eDirection::LEFT)
	//	{
	//		veloc.x = -NORMAL_BULLET_SPEED;
	//	}
	//	else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
	//	{
	//		veloc.x = NORMAL_BULLET_SPEED;
	//	}
	//	else
	//	{
	//		veloc.x = 0;
	//	}

	//	if ((_direction & eDirection::TOP) == eDirection::TOP)
	//	{
	//		veloc.y = NORMAL_BULLET_SPEED;
	//	}
	//	else if ((_direction & eDirection::BOTTOM) == eDirection::BOTTOM)
	//	{
	//		veloc.y = -NORMAL_BULLET_SPEED;
	//	}
	//	else
	//	{
	//		veloc.y = 0;
	//	}
	//}
	//else
	//{
	//	auto rad = _degree * M_PI / 180;
	//	veloc.x = sin(rad) * NORMAL_BULLET_SPEED;
	//	veloc.y = cos(rad) * NORMAL_BULLET_SPEED;
	//}

	auto movement = new Movement(GVector2(0, 0), veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement",movement));
	//_componentList["Movement"] = movement;

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody",collisionBody));
	//_componentList.find("CollisionBody")->second = collisionBody;
	//_componentList["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

// KHởi tạo vận tốc ban đầu cho đạn. (copy từ đoạn code trên)
GVector2 Bullet::initveloc(float bullet_speed)
{
	GVector2 result;
	if (_direction != eDirection::NONE)
	{
		if ((_direction & eDirection::LEFT) == eDirection::LEFT)
		{
			result.x = -bullet_speed;
		}
		else if ((_direction & eDirection::RIGHT) == eDirection::RIGHT)
		{
			result.x = bullet_speed;
		}
		else
		{
			result.x = 0;
		}

		if ((_direction & eDirection::TOP) == eDirection::TOP)
		{
			result.y = bullet_speed;
		}
		else if ((_direction & eDirection::BOTTOM) == eDirection::BOTTOM)
		{
			result.y = -bullet_speed;
		}
		else
		{
			result.y = 0;
		}
	}
	else
	{
		auto rad = _degree * M_PI / 180;
		result.x = sin(rad) * bullet_speed;
		result.y = cos(rad) * bullet_speed;
	}
	return result;

}

float Bullet::getDegree()
{
	return _degree;
}

void Bullet::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	
	// viewport
	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();

	// Nếu ra khỏi màn hình thì có trạng thái là destroy.
	if (viewport->isContains(this->getBounding()) == false)
		this->setStatus(eStatus::DESTROY);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Bullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_sprite->render(spriteHandle, viewport);
}

void Bullet::release()
{
	for each (auto item in _componentList)
	{
		SAFE_DELETE(item.second);
	}
	_componentList.clear();
}

int Bullet::getDamage()
{
	return _damage;
}

void Bullet::setDamge(int dmg)
{
	this->_damage = dmg;
}
GVector2 Bullet::getVelocity()
{
	auto move = (Movement*)this->_componentList.find("Movement")->second;
	//auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

bool Bullet::isBillBullet()
{
	return ((_type & eBulletType::BILL_BULLET) == (eBulletType::BILL_BULLET));
}

bool Bullet::isEnemyBullet()
{
	return ((_type & eBulletType::ENEMY_BULLET) == (eBulletType::ENEMY_BULLET));
}

bool Bullet::isContainType(eBulletType type)
{
	return ((_type & type) == type);
}

void Bullet::onCollisionBegin(CollisionEventArg* collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();

	if (this->isBillBullet())
	{
		// Nếu đây là đạn của Bill
		switch (objectID)
		{
		case AIRCRAFT:
			if (collision_arg->_otherObject->getStatus() == eStatus::HIDING || collision_arg->_otherObject->getStatus() == eStatus::EXPLORED)
				return;
			collision_arg->_otherObject->setStatus(eStatus::BURST);
			if (this->_type != eBulletType::L_BULLET)
			this->setStatus(eStatus::DESTROY);
			break;
		case FALCON:
			if (collision_arg->_otherObject->getStatus() == eStatus::EXPLORED)
				return;
			if (((Falcon*)collision_arg->_otherObject)->isOpenned() == false)
				return;
			collision_arg->_otherObject->setStatus(eStatus::BURST);
			if (this->_type != eBulletType::L_BULLET)
			this->setStatus(eStatus::DESTROY);
			break;
		case SOLDIER: case RIFLEMAN:
			if (collision_arg->_otherObject->getStatus() != HIDDEN && collision_arg->_otherObject->getStatus() != EXPOSING)
				((BaseEnemy*)collision_arg->_otherObject)->dropHitpoint(_damage);
			// Đạn laser đi xuyên qua soldier và rifleman
			if (this->_type != eBulletType::L_BULLET)
				this->setStatus(eStatus::DESTROY);
			break;
		case BOSS_SHIELD:
		case BOSS_GUN:
		case REDCANNON:
		case WALL_TURRET:
			((BaseEnemy*)collision_arg->_otherObject)->dropHitpoint(_damage);
			this->setStatus(eStatus::DESTROY);
			if (this->isContainType(eBulletType::L_BULLET) == true && ((BaseEnemy*)collision_arg->_otherObject)->getHitpoint() <= 0)
				this->setStatus(eStatus::NORMAL);
			SoundManager::getInstance()->Play(eSoundId::ATTACK_CANNON);
			break;
		}
	}
	
	if (this->isEnemyBullet())
	{
		switch (objectID)
		{
		case BILL:
		{
			if (collision_arg->_otherObject->isInStatus(eStatus::DYING) == false)
			{
				collision_arg->_otherObject->setStatus(eStatus::DYING);
				((Bill*)collision_arg->_otherObject)->die();
			}
			break;
		}
		case LAND:
		{
			if (this->isContainType(eBulletType::BOSSSTAGE1_BULLET))
			{
				this->setStatus(eStatus::BURST);
			}
		}
		}
	}
}

float Bullet::checkCollision(BaseObject * object, float dt)
{
	auto body = (CollisionBody*)_componentList.find("CollisionBody")->second;
	//auto body = (CollisionBody*)_componentList["CollisionBody"];
	if (object->getId() == eID::BULLET)
		return 0.0f;
	body->checkCollision(object, dt);

	return 0.0f;
}
