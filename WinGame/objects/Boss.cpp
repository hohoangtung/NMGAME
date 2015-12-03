#include "Boss.h"
#include "Explosion.h"

#pragma region Boss

void Boss::init()
{
	_gun1 = new BossGun(GVector2(_startposition.x + 64.0f, _startposition.y + 154.0f));
	_gun1->init();

	_gun2 = new BossGun(GVector2(_startposition.x + 22.0f, _startposition.y + 154.0f));
	_gun2->init();

	_shield = new BossShield(GVector2(_startposition.x + 32.0f, _startposition.y + 64.0f));
	_shield->init();
}

void Boss::update(float deltatime)
{
	if (_gun1 != nullptr)
	{
		_gun1->update(deltatime);
	}
	if (_gun2 != nullptr)
	{
		_gun2->update(deltatime);
	}
	if (_shield != nullptr)
	{
		_shield->update(deltatime);
	}
}

void Boss::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_gun1 != nullptr)
	{
		_gun1->draw(spriteHandle, viewport);
	}
	if (_gun2 != nullptr)
	{
		_gun2->draw(spriteHandle, viewport);
	}
	if (_shield != nullptr)
	{
		_shield->draw(spriteHandle, viewport);
	}
}

void Boss::release()
{
	if (_gun1 != nullptr)
	{
		_gun1->release();
		delete _gun1;
		_gun1 = nullptr;
	}
	if (_gun2 != nullptr)
	{
		_gun2->release();
		delete _gun2;
		_gun2 = nullptr;
	}
	if (_shield != nullptr)
	{
		_shield->release();
		delete _shield;
		_shield = nullptr;
	}
}

float Boss::checkCollision(BaseObject* object, float dt)
{
	_gun1->checkCollision(object, dt);
	_gun2->checkCollision(object, dt);
	return 0.0f;
}

BaseObject* Boss::getGun1()
{
	return _gun1;
}

BaseObject* Boss::getGun2()
{
	return _gun2;
}

BaseObject* Boss::getShield()
{
	return _shield;
}

RECT Boss::getBounding()
{
	return RECT{ 0 };
}

Boss::Boss(GVector2 position) : BaseEnemy(eID::BOSS_STAGE1)
{
	_startposition = position;
}

Boss::~Boss()
{
}

#pragma endregion 

#pragma region BossBullet

Boss::BossBullet::BossBullet(GVector2 startposition, GVector2 force, GVector2 gravity) : Bullet(startposition, eBulletType(ENEMY_BULLET | BOSSSTAGE1_BULLET), 0.0f)
{
	_startPosition = startposition;
	_force = force;
	_gravity = gravity;
}

void Boss::BossBullet::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "boss1_bullet"));
	_sprite->setScale(SCALE_FACTOR);
	this->setPosition(_startPosition);
	Movement* movement = new Movement(VECTOR2ZERO, _force, _sprite);
	Gravity* gravity = new Gravity(_gravity, movement);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_componentList["Movement"] = movement;
	this->_componentList["Gravity"] = gravity;
	this->_componentList["CollisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);

	this->setPhysicsBodySide(eDirection::ALL);
	_stopWatch = new StopWatch();
}

void Boss::BossBullet::update(float deltatime)
{
	auto status = this->getStatus();
	if (status == eStatus::BURST)
	{
		this->setStatus(eStatus::NORMAL);
		this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "burst"));
		auto movement = (Movement*)_componentList["Movement"];
		movement->setVelocity(VECTOR2ZERO);
		auto gravity = (Gravity*)_componentList["Gravity"];
		gravity->setGravity(VECTOR2ZERO);
		this->setPhysicsBodySide(eDirection::NONE);
	}
	if (this->getStatus() == eStatus::NORMAL)
	{
		auto movement = (Movement*)_componentList["Movement"];
		auto veloc = movement->getVelocity();
		if (veloc.x == VECTOR2ZERO.x && veloc.y == VECTOR2ZERO.y)
		{
			if (_stopWatch->isStopWatch(1.2))
			{
				this->setStatus(eStatus::DESTROY);
				return;
			}
		}
	}
	Bullet::update(deltatime);
}

#pragma endregion 

#pragma region BossGun

void Boss::BossGun::dropHitpoint()
{
	_hitpoint--;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}

void Boss::BossGun::dropHitpoint(int damage)
{
	_hitpoint -= damage;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}

Boss::BossBullet::~BossBullet()
{
}

void Boss::BossGun::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	this->setPosition(_startposition);
	_animation = new Animation(_sprite, 0.5f);
	_animation->addFrameRect(eID::BOSS_STAGE1, "gun_1", "gun_2", NULL);

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
	//__hook(&CollisionBody::onCollisionBegin, collisionBody, &Boss::BossGun::onCollisionBegin);

	_stopWatch = new StopWatch();

	this->setScale(SCALE_FACTOR);

	_forceBullet.push_back(pair<GVector2, GVector2>(GVector2(-100.0f, 30.0f), GVector2(0.0f, -200.0f)));
	_forceBullet.push_back(pair<GVector2, GVector2>(GVector2(-220.0f, 50.0f), GVector2(0.0f, -200.0f)));

}

void Boss::BossGun::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eStatus status = this->getStatus();
	if (status == eStatus::NORMAL)
	{
		_animation->update(deltatime);
		if (_stopWatch->isTimeLoop(1000.0f))
		{
			int index = rand() %_forceBullet.size();
			auto pair = _forceBullet.at(index);
			GVector2 startposition = GVector2(this->getBounding().left, this->getPosition().y);
			BulletManager::insertBullet(new Boss::BossBullet(startposition, pair.first, pair.second));
		}
	}
	if (status == eStatus::BURST)
	{
		if (_explosion == nullptr)
		{
			this->_explosion = new Explosion(2);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(this->getPosition());
		}
		else
		{
			this->_explosion->update(deltatime);
			if (_explosion->getStatus() == eStatus::DESTROY)
			{
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
}

void Boss::BossGun::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::NORMAL)
	{
		_animation->draw(spriteHandle, viewport);
	}
	else if (this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
}

void Boss::BossGun::release()
{
	SAFE_DELETE(_stopWatch);
	SAFE_DELETE(_animation);
	SAFE_DELETE(_sprite);
	if (_explosion != nullptr)
	{
		_explosion->release();
		delete _explosion;
		_explosion = nullptr;
	}
}

Boss::BossGun::BossGun(GVector2 position) : BaseEnemy(eID::BOSS_GUN)
{
	_startposition = position;
}

Boss::BossGun::~BossGun()
{
}

#pragma endregion

#pragma region BossShield

void Boss::BossShield::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	_animation = new Animation(_sprite, 0.09f);
	this->setPosition(_startposition);
	this->setScale(SCALE_FACTOR);
	_animation->addFrameRect(eID::BOSS_STAGE1, "shield_1", "shield_2", "shield_3", NULL);
	_explosion = nullptr;
}
void Boss::BossShield::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eStatus status = this->getStatus();
	if (status == eStatus::NORMAL)
	{
		_animation->update(deltatime);
	}
	if (status == eStatus::BURST)
	{
		if (_explosion == nullptr)
		{
			this->_explosion = new Explosion(2);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(this->getPosition());
		}
		else
		{
			this->_explosion->update(deltatime);
			if (_explosion->getStatus() == eStatus::DESTROY)
			{
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
}
void Boss::BossShield::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::NORMAL)
	{
		_animation->draw(spriteHandle, viewport);
	}
	else if (this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
}
void Boss::BossShield::release()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(_sprite);
	if (_explosion != nullptr)
	{
		_explosion->release();
		delete _explosion;
		_explosion = nullptr;
	}
}

void Boss::BossShield::dropHitpoint()
{
	_hitpoint--;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}
void Boss::BossShield::dropHitpoint(int damage)
{
	_hitpoint -= damage;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}
Boss::BossShield::BossShield(GVector2 position) : BaseEnemy(eID::BOSS_SHIELD)
{
	_startposition = position;
}
Boss::BossShield::~BossShield()
{

}

#pragma endregion