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

	_stopwatch = new StopWatch();
	_movingWatch = new StopWatch();
	_shot = false;
	_bounce = 1.0f;
	_animationTime = 0.0f;
	_awake = false;
	this->setStatus(eStatus::HIDDEN);
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
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	if (this->getPositionY() < bill->getPositionY() - 96.0f)
		_awake = true;
	if (!_shot && _awake)
		this->setStatus(eStatus::SHOOTING);
	else
	{
		float time = GameTime::getInstance()->getTotalGameTime() - _animationTime;
		if (time > 800.0f)
			this->setStatus(eStatus::HIDDEN);
	}
	if (_stopwatch->isTimeLoop(SCUBASOLDIER_SHOOTING_DELAY) && _shot)
		_shot = false;
	if (this->getStatus() == eStatus::SHOOTING && !_shot)
	{
		BulletManager::insertBullet(new ScubaBullet(this->getPosition(), GVector2(0, 448.0f), GVector2(0, -320.0f)));
		_animationTime = GameTime::getInstance()->getTotalGameTime();
		_shot = true;		
	}
	if (_movingWatch->isTimeLoop(300.0f))
	{
		this->setPositionY(this->getPositionY() - _bounce);
		_bounce = -_bounce;
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

RECT ScubaSoldier::getBounding()
{
	RECT basebound = BaseObject::getBounding();
	basebound.top -= 15 * this->getScale().y;
	return basebound;
}




ScubaSoldier::ScubaBullet::ScubaBullet(GVector2 startposition, GVector2 force, GVector2 gravity) : Bullet(startposition, eBulletType(SCUBABULLET | ENEMY_BULLET), 0.0f)
{
	_startPosition = startposition;
	_force = force;
	_gravity = gravity;
}

void ScubaSoldier::ScubaBullet::init()
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
	_explosionburst = nullptr;
	_explosion = nullptr;
	if (this->_force.x == 0)
	{
		_tripleFlag = false;
		_canFireFlag = true;
	}
	else
	{
		_tripleFlag = true;
		_canFireFlag = false;
	}
}

void ScubaSoldier::ScubaBullet::update(float deltatime)
{
	auto status = this->getStatus();
	if (status == eStatus::BURST)
	{
		auto movement = (Movement*)_componentList["Movement"];
		movement->setVelocity(VECTOR2ZERO);
		auto gravity = (Gravity*)_componentList["Gravity"];
		gravity->setGravity(VECTOR2ZERO);
		if (_explosion == nullptr)
		{
			this->_explosion = new Explosion(1);
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
	else
	{
		this->tripleAttack();
		this->fireBurst(deltatime);
	}

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
void ScubaSoldier::ScubaBullet::tripleAttack()
{
	auto movement = (Movement*)_componentList["Movement"];
	if (_tripleFlag == false && movement->getVelocity().y <= 0)
	{
		_tripleFlag = true;
		BulletManager::insertBullet(new ScubaBullet(this->getPosition(), GVector2(112, movement->getVelocity().y), GVector2(0, -320.0f))); // hard code. should define
		BulletManager::insertBullet(new ScubaBullet(this->getPosition(), GVector2(-112, movement->getVelocity().y), GVector2(0, -320.0f))); // hard code. should define
	}
}
void ScubaSoldier::ScubaBullet::fireBurst(float deltatime)
{
	auto movement = (Movement*)_componentList["Movement"];
	if (_canFireFlag == true && movement->getVelocity().y <= 170)
	{
		_canFireFlag = false;
		_explosionburst = new Explosion(2);
		_explosionburst->init();
		_explosionburst->setScale(SCALE_FACTOR);
		_explosionburst->setPosition(this->getPosition());
		SoundManager::getInstance()->Play(eSoundId::MBULLET_FIRE);

	}
	else
	{
		if (_explosionburst != nullptr )
		{
			if (_explosionburst->getStatus() == eStatus::DESTROY)
			{
				_explosionburst->release();
				delete _explosion;
				_explosion = nullptr;
			}
			if (_explosionburst->getStatus() == eStatus::NORMAL)
			{
				_explosionburst->update(deltatime);
			}
		}
	}
}

void ScubaSoldier::ScubaBullet::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::NORMAL)
	{
		_sprite->render(spriteHandle, viewport);
	}
	else if (_explosion != nullptr && this->getStatus() == eStatus::BURST)
	{
		_explosion->draw(spriteHandle, viewport);
	}
	if (_explosionburst != nullptr)
	{
		_explosionburst->draw(spriteHandle, viewport);
	}
}

float ScubaSoldier::ScubaBullet::checkCollision(BaseObject* object, float dt)
{
	auto body = (CollisionBody*)_componentList.find("CollisionBody")->second;
	if (object->getId() == eID::LAND || object->getId() == eID::BILL)
	{
		body->checkCollision(object, dt, false);
	}
	return 0.0f;
}

void ScubaSoldier::ScubaBullet::release()
{

}

ScubaSoldier::ScubaBullet::~ScubaBullet()
{
}