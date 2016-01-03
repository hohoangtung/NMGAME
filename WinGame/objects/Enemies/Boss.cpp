#include "Boss.h"
#include "../Explosion.h"

#pragma region Boss

void Boss::init()
{
	// Cục súng bên phải
	_gun1 = new BossGun(GVector2(_startposition.x + 74.0f, _startposition.y + 154.0f), 0);
	_gun1->init();
	_moulder1 = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	_moulder1->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "moulder_1"));
	_moulder1->setScale(SCALE_FACTOR);
	_moulder1->setOrigin(_gun1->getOrigin());
	_moulder1->setPosition(_gun1->getPosition());

	// Cục súng bên trái
	_gun2 = new BossGun(GVector2(_startposition.x + 30.0f, _startposition.y + 154.0f), 1);
	_gun2->init();
	_moulder2 = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	_moulder2->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "moulder_2"));
	_moulder2->setScale(SCALE_FACTOR);
	_moulder2->setOrigin(_gun2->getOrigin());
	_moulder2->setPosition(_gun2->getPosition());

	_shield = new BossShield(GVector2(_startposition.x + 14.0f, _startposition.y + 65.0f));
	_shield->init();
	_bigmoudler = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	_bigmoudler->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "big_moulder"));
	_bigmoudler->setScale(SCALE_FACTOR);
	_bigmoudler->setOrigin(_shield->getOrigin());
	_bigmoudler->setPosition(_shield->getPosition());

	// Thanh niên núp lùm
	_rifleman = new Rifleman(eStatus::HIDDEN, _startposition.x + 32.0f, _startposition.y + 282.0f);
	_rifleman->init();
	//auto gravity = ((Rifleman*)_rifleman)->getComponent("Gravity");
	//((Gravity*)gravity)->setGravity(VECTOR2ZERO);

	_octexplose = nullptr;

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));
	this->setPhysicsBodySide(eDirection::ALL);
}

void Boss::update(float deltatime)
{
	if (_gun1 != nullptr)
	{
		_gun1->update(deltatime);
		if (_gun1->getStatus() == eStatus::DESTROY)
		{
			_gun1->release();
			delete _gun1;
			_gun1 = nullptr;
		}
	}
	if (_gun2 != nullptr)
	{
		_gun2->update(deltatime);
		if (_gun2->getStatus() == eStatus::DESTROY)
		{
			_gun2->release();
			delete _gun2;
			_gun2 = nullptr;
		}
	}
	if (_shield != nullptr)
	{
		_shield->update(deltatime);
		if (_shield->getStatus() == eStatus::BURST)
		{
			if (_gun1 != nullptr && _gun1->getStatus() != eStatus::BURST)
			{
				_gun1->setStatus(eStatus::BURST);
			}
			if (_gun2 != nullptr && _gun2->getStatus() != eStatus::BURST)
			{
				_gun2->setStatus(eStatus::BURST);
			}
			if (_rifleman != nullptr && _rifleman->getStatus() != eStatus::BURST)
			{ 
				((Rifleman*)_rifleman)->die();
			}
		}
		if (_shield->getStatus() == eStatus::DESTROY && _octexplose == nullptr)
		{
			_octexplose = new OctExplose(GVector2(_shield->getPositionX(), _shield->getPositionY()));
			_octexplose->init();
			SoundManager::getInstance()->Stop(eSoundId::BACKGROUND_STAGE1);
			SoundManager::getInstance()->Play(eSoundId::DESTROY_BOSS);
		}
	}
	if (_rifleman != nullptr)
	{
		_rifleman->update(deltatime);
	}
	if (_octexplose != nullptr)
	{
		_octexplose->update(deltatime);
		if (_octexplose->getStatus() == eStatus::DESTROY)
		{
			this->setStatus(eStatus::DYING);
			this->setPhysicsBodySide(eDirection::NONE);
		}
	}

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}
}

void Boss::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_gun1 != nullptr)
	{
		if (_gun1->getStatus() == eStatus::DESTROY)
		{
			_moulder1->render(spriteHandle, viewport);
		}
		else
		{
			_gun1->draw(spriteHandle, viewport);
		}
	}
	if (_gun2 != nullptr)
	{
		if (_gun2->getStatus() == eStatus::DESTROY)
		{
			_moulder2->render(spriteHandle, viewport);
		}
		else
		{
			_gun2->draw(spriteHandle, viewport);
		}
	}
	if (_shield != nullptr)
	{
		if (_shield->getStatus() == eStatus::DESTROY)
		{
			_bigmoudler->render(spriteHandle, viewport);
			if (_octexplose != nullptr)
			{
				_octexplose->draw(spriteHandle, viewport);
			}
		}
		else
		{
			_shield->draw(spriteHandle, viewport);
		}
	}
	if (_rifleman != nullptr)
	{
		_rifleman->draw(spriteHandle, viewport);
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
	if (_rifleman != nullptr)
	{
		_rifleman->release();
		delete _rifleman;
		_rifleman = nullptr;
	}
	if (_octexplose != nullptr)
	{
		_octexplose->release();
		delete _octexplose;
		_octexplose = nullptr;
	}
	SAFE_DELETE(_moulder1);
	SAFE_DELETE(_moulder2);
	SAFE_DELETE(_bigmoudler);
}

float Boss::checkCollision(BaseObject* object, float dt)
{

	//_gun1->checkCollision(object, dt);
	//_gun2->checkCollision(object, dt);

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
BaseObject* Boss::getRifleMan()
{
	return _rifleman;
}

RECT Boss::getBounding()
{
	RECT rect;
	rect.left = this->_startposition.x;
	rect.bottom = this->_startposition.y;
	rect.top = rect.bottom + _height;
	rect.right = rect.left + 200.0f; // hard code
	return rect;
}

Boss::Boss(GVector2 position, int height) : BaseEnemy(eID::BOSS_STAGE1)
{
	_startposition = position;
	_height = height;
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

Boss::BossBullet::~BossBullet()
{
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
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin); //dư?

	this->setPhysicsBodySide(eDirection::ALL);
	_stopWatch = new StopWatch(); //dư?
}

void Boss::BossBullet::update(float deltatime)
{
	auto status = this->getStatus();
	if (status == eStatus::BURST)
	{
		auto movement = (Movement*)_componentList["Movement"];
		movement->setVelocity(VECTOR2ZERO);
		auto gravity = (Gravity*)_componentList["Gravity"];
		gravity->setGravity(VECTOR2ZERO);
		this->setPhysicsBodySide(eDirection::NONE);
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

	Bullet::update(deltatime);
}

void Boss::BossBullet::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::NORMAL)
	{
		_sprite->render(spriteHandle, viewport);
	}
	else if (_explosion != nullptr && this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
}

float Boss::BossBullet::checkCollision(BaseObject* object, float dt)
{
	auto body = (CollisionBody*)_componentList.find("CollisionBody")->second;
	if (object->getId() == eID::LAND)
	{
		if (((Land*)object)->canJump() == false)
		{
			body->checkCollision(object, dt, false);
		}
	}
	else if (object->getId() == eID::BILL)
	{
		body->checkCollision(object, dt, false);
	}

	return 0.0f;
}

RECT Boss::BossBullet::getBounding()
{
	if (this->getStatus () == NORMAL)
	{
		return BaseObject::getBounding();
	}
	else if (_explosion != nullptr && this->getStatus() == eStatus::BURST)
	{
		return _explosion->getBounding();
	}
	return BaseObject::getBounding();
}

#pragma endregion 

#pragma region BossGun

void Boss::BossGun::dropHitpoint()
{
	_hitpoint--;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
		gainScore();
	}
}

void Boss::BossGun::dropHitpoint(int damage)
{
	_hitpoint -= damage;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
		gainScore();
	}
}

void Boss::BossGun::init()
{
	this->_hitpoint = BOSS_GUN_HP;
	this->setScore(GUN_SCORE);
	_sprite = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	this->setPosition(_startposition);
	//_animation = new Animation(_sprite, 0.5f);

	this->initFrameRect();

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;
	//__hook(&CollisionBody::onCollisionBegin, collisionBody, &Boss::BossGun::onCollisionBegin);

	_stopWatch = new StopWatch();

	this->setScale(SCALE_FACTOR);
	this->setOrigin(GVector2(1.0f, 0.5f));

	_forceBullet.push_back(pair<GVector2, GVector2>(GVector2(-50.0f, 100.0f), GVector2(0.0f, -450.0f)));
	_forceBullet.push_back(pair<GVector2, GVector2>(GVector2(-100.0f, 120.0f), GVector2(0.0f, -420.0f)));
	_forceBullet.push_back(pair<GVector2, GVector2>(GVector2(-130.0f, 150.0f), GVector2(0.0f, -400.0f)));

}

void Boss::BossGun::initFrameRect()
{
	if (_statusGun & 1)
	{
		//_statusGun is Odd
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "gun_2"));
	}
	else
	{
		//_statussGun is Even
		_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "gun_1"));
	}

	//if (_statusGun % 2 == 0)
	//{
	//	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "gun_1"));
	//	//_animation->addFrameRect(eID::BOSS_STAGE1, "gun_1", "gun_2", NULL);
	//}
	//else
	//{
	//	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BOSS_STAGE1, "gun_2"));
	//	//_animation->addFrameRect(eID::BOSS_STAGE1, "gun_2", "gun_1", NULL);
	//}
}


void Boss::BossGun::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	eStatus status = this->getStatus();
	if (status == eStatus::NORMAL)
	{
		//_animation->update(deltatime);
		if (_stopWatch->isTimeLoop(750.0f))
		{
			_statusGun++;
			this->initFrameRect();
			if ((_statusGun & 1) == 0)
			{
				int index = rand() % _forceBullet.size();
				auto pair = _forceBullet.at(index);
				GVector2 startposition = GVector2(this->getBounding().left, this->getPosition().y);
				BulletManager::insertBullet(new Boss::BossBullet(startposition, pair.first, pair.second));
			}
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
	//	_animation->draw(spriteHandle, viewport);
		_sprite->render(spriteHandle, viewport);
	}
	else if (_explosion != nullptr && this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
}

void Boss::BossGun::release()
{
	SAFE_DELETE(_stopWatch);
	SAFE_DELETE(_animation);
	if (_explosion != nullptr)
	{
		_explosion->release();
		delete _explosion;
		_explosion = nullptr;
	}
	SAFE_DELETE(_sprite);
}

Boss::BossGun::BossGun(GVector2 position, int startStatus) : BaseEnemy(eID::BOSS_GUN)
{
	_startposition = position;
	_statusGun = startStatus;
}

Boss::BossGun::~BossGun()
{
}

#pragma endregion

#pragma region BossShield

void Boss::BossShield::init()
{
	this->_hitpoint = BOSS_SHIELD_HP;
	this->setScore(SHIELD_SCORE);
	_sprite = SpriteManager::getInstance()->getSprite(eID::BOSS_STAGE1);
	_animation = new Animation(_sprite, 0.09f);
	this->setPosition(_startposition);
	this->setScale(SCALE_FACTOR);
	this->setOrigin(GVector2(0.0f, 0.5f));
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
	else if (_explosion != nullptr && this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
}
void Boss::BossShield::release()
{
	SAFE_DELETE(_animation);
	if (_explosion != nullptr)
	{
		_explosion->release();
		delete _explosion;
		_explosion = nullptr;
	}
	SAFE_DELETE(_sprite);
}

void Boss::BossShield::dropHitpoint()
{
	_hitpoint--;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
		gainScore();
	}
}
void Boss::BossShield::dropHitpoint(int damage)
{
	_hitpoint -= damage;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
		gainScore();
	}
}
RECT Boss::BossShield::getBounding()
{
	RECT basebound = BaseObject::getBounding();
	basebound.top -= 18;
	basebound.bottom += 14;
	basebound.right += 8;
	basebound.left -= 18;
	return basebound;
}


Boss::BossShield::BossShield(GVector2 position) : BaseEnemy(eID::BOSS_SHIELD)
{
	_startposition = position;
}
Boss::BossShield::~BossShield()
{

}

#pragma endregion

#pragma region OctExplose

Boss::OctExplose::OctExplose(GVector2 position) : BaseObject(eID::QUADEXPLODE)
{
	_transform = new Transformable();
	this->_transform->setPosition(position);
}
void Boss::OctExplose::init()
{
	_timer = 0.0f;
	auto pos = _transform->getPosition();

	GVector2 position;
	for (int i = 0; i < 8; i++)
	{
		_explosion[i] = new Explosion(2);
		_explosion[i]->init();
		_explosion[i]->setScale(SCALE_FACTOR);
		_explosion[i]->setOrigin(GVector2(0.0f, 0.5f));

		position.x =  64 * (i / 2) + 4;
		position.y = 64 * (i % 2) + 4;

		_explosion[i]->setPosition(GVector2(pos.x + position.x, pos.y + position.y));

	}
}

void Boss::OctExplose::update(float deltatime)
{
	_timer += deltatime;

	int index = _timer /  100;
	for (int i = 0; i < min(index, 8); i++)
	{
		_explosion[i]->update(deltatime);
		if (_explosion[7] ->getStatus() == eStatus::DESTROY)
		{
			this->setStatus(eStatus::DESTROY);
		}
	}
}
void Boss::OctExplose::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->getStatus() != eStatus::NORMAL)
		return;

	int index = _timer / 90;
	for (int i = 0; i < min(index, 8); i++)
	{
		_explosion[i]->draw(spritehandle , viewport);
	}
}

GVector2 Boss::OctExplose::getPosition()
{
	return	this->_transform->getPosition();
}

void Boss::OctExplose::release()
{
	for (int i = 0; i < 8; i++)
	{
		if (_explosion[i] != nullptr)
		{
			_explosion[i]->release();
			delete _explosion[i];
			_explosion[i] = nullptr;
		}
	}
	SAFE_DELETE(_transform);
}

#pragma endregion