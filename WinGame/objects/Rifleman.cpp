#include "Rifleman.h" 

float animationTime = 0;
#define PI 3.14159265
Rifleman::Rifleman(eStatus status, GVector2 position) : BaseEnemy(eID::RIFLEMAN) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::RIFLEMAN);
	_sprite->setFrameRect(0, 0, 23, 38);
	this->setStatus(status);
	this->setPosition(position);
}

Rifleman::Rifleman(eStatus status, float x, float y) : BaseEnemy(eID::RIFLEMAN) {
	_sprite = SpriteManager::getInstance()->getSprite(eID::RIFLEMAN);
	_sprite->setFrameRect(0, 0, 23, 38);
	GVector2 pos(x, y);
	this->setStatus(status);
	this->setPosition(pos);
}

Rifleman::~Rifleman() {}

void Rifleman::init()
{
	this->setScale(SCALE_FACTOR);

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;	

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Rifleman::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Rifleman::onCollisionEnd);

	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Gravity", new Gravity(GVector2(0, -ENEMY_GRAVITY), (Movement*)(this->getComponent("Movement")))));
	_animations[NORMAL] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[NORMAL]->addFrameRect(eID::RIFLEMAN, "normal_01", NULL);

	_animations[NORMAL | SHOOTING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[NORMAL | SHOOTING]->addFrameRect(eID::RIFLEMAN, "shoot_01", "normal_01", NULL);

	_animations[AIMING_UP] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[AIMING_UP]->addFrameRect(eID::RIFLEMAN, "aim_up_01", NULL);

	_animations[AIMING_DOWN] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[AIMING_DOWN]->addFrameRect(eID::RIFLEMAN, "aim_down_01", NULL);

	_animations[AIMING_UP | SHOOTING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[AIMING_UP | SHOOTING]->addFrameRect(eID::RIFLEMAN, "aim_up_01", "shoot_up_01", NULL);

	_animations[AIMING_DOWN | SHOOTING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[AIMING_DOWN | SHOOTING]->addFrameRect(eID::RIFLEMAN, "aim_down_01", NULL);

	_animations[HIDDEN] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[HIDDEN]->addFrameRect(eID::RIFLEMAN, "hide_01", NULL);

	_animations[EXPOSING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[EXPOSING]->addFrameRect(eID::RIFLEMAN, "hide_01", "expose_01", "expose_02", NULL);

	_animations[HIDING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[HIDING]->addFrameRect(eID::RIFLEMAN, "expose_02", "expose_01", "hide_01", NULL);

	_animations[EXPOSING | SHOOTING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[EXPOSING | SHOOTING]->addFrameRect(eID::RIFLEMAN, "expose_02", NULL);

	_stopwatch = new StopWatch();
	_loopwatch = new StopWatch();
	this->setHitpoint(RIFLEMAN_HITPOINT);
	this->setScore(RIFLEMAN_SCORE);
}

void Rifleman::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	this->_sprite->render(spritehandle, viewport);
	_animations[this->getStatus()]->draw(spritehandle, viewport);

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->draw(spritehandle, viewport);
	}
}

void Rifleman::release()
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

IComponent* Rifleman::getComponent(string componentName)
{
	return _listComponent.find(componentName)->second;
}

void Rifleman::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}

void Rifleman::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}

bool Rifleman::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}

void Rifleman::setShootingAngle(double angle) 
{
	this->_shootingAngle = angle;
}
void Rifleman::update(float deltatime)
{ 
	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getHitpoint() <= 0) 
	{
		if (this->isInStatus(SHOOTING))
			this->setStatus(NORMAL);
		this->die();
		if (this->_stopwatch->isStopWatch(200)) 
		{
			auto pos = this->getPosition();
			Movement *movement = (Movement*)this->getComponent("Movement");
			movement->setVelocity(GVector2(0, 0));
			_explosion = new Explosion(1);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(pos);
			this->setStatus(eStatus::DESTROY);
			return;
		}
	}
	if (!this->isInStatus(eStatus::EXPOSING) && !this->isInStatus(eStatus::HIDING) && !this->isInStatus(eStatus::HIDDEN))
	{
		if (_shootingAngle >= 70 && _shootingAngle <= 110)
		{
			this->setScaleX(-SCALE_FACTOR);
			this->setStatus(NORMAL);
		}
		else if (_shootingAngle >= 0 && _shootingAngle < 70)
		{
			this->setScaleX(-SCALE_FACTOR);
			this->setStatus(AIMING_UP);
		}
		else if (_shootingAngle < 0 && _shootingAngle >= -70)
		{
			this->setScaleX(SCALE_FACTOR);
			this->setStatus(AIMING_UP);
		}
		else if (_shootingAngle < -70 && _shootingAngle >= -110)
		{
			this->setScaleX(SCALE_FACTOR);
			this->setStatus(NORMAL);
		}
		else if (_shootingAngle < -110 && _shootingAngle >= -180)
		{
			this->setScaleX(SCALE_FACTOR);
			this->setStatus(AIMING_DOWN);
		}
		else if (_shootingAngle >= 110 && _shootingAngle < 180)
		{
			this->setScaleX(-SCALE_FACTOR);
			this->setStatus(AIMING_DOWN);
		}
		this->addStatus(SHOOTING);
		calculateShootingAngle();
	}
	else 
	{
		float time = GameTime::getInstance()->getTotalGameTime() - animationTime;
		calculatingShootingDirection();
		if (this->getStatus() == HIDDEN)
		{
			if (time > 2000.0f)
			{
				this->setStatus(EXPOSING);
				_animations[EXPOSING]->setIndex(0);
			}
		}
		else if (this->getStatus() == EXPOSING)
		{
			if (time > 2700.0f)
			{
				this->addStatus(SHOOTING);
			}
		}
		else if (this->isInStatus(SHOOTING))
		{
			if (time > 7800.0f)
			{
				this->setStatus(HIDING);				
				_animations[HIDING]->setIndex(0);
			}
		}
		else if (this->getStatus() == HIDING) {
			if (time > 8200.0f)
			{
				this->setStatus(HIDDEN);
				animationTime = GameTime::getInstance()->getTotalGameTime();
			}
		}
	}
	if (_loopwatch->isTimeLoop(RIFLEMAN_SHOOTING_DELAY))
	{
		if (this->isInStatus(SHOOTING))
			shoot();
	}
	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->update(deltatime);
	}

	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}

	_animations[this->getStatus()]->update(deltatime);
}

void Rifleman::setStatus(eStatus status) 
{
	if (_status != status)
		_status = status;
}
void Rifleman::calculateShootingAngle() {
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = (this->getPosition().x) - (bill->getPosition().x);
	float dy = (this->getPosition().y) - (bill->getPosition().y + bill->getSprite()->getFrameHeight() / 2);
	if (dx > 0 && dy < 0)
		_shootingAngle = atan(dx / dy) * 180 / PI;
	else if (dx > 0 && dy > 0)
		_shootingAngle = -atan(dy / dx) * 180 / PI - 90;
	else if (dx < 0 && dy < 0)
		_shootingAngle = atan(dx / dy) * 180 / PI;
	else if (dx < 0 && dy > 0)
		_shootingAngle = -atan(dy / dx) * 180 / PI + 90;
	else if (dx == 0 && dy > 0)
		_shootingAngle = 0;
	else if (dx == 0 && dy < 0)
		_shootingAngle = 180;
	else if (dx < 0 && dy == 0)
		_shootingAngle = -90;
	else if (dx > 0 && dy == 0)
		_shootingAngle = 90;
}
float Rifleman::getShootingAngle() 
{
	return _shootingAngle;
}

void Rifleman::onCollisionBegin(CollisionEventArg* collision_event) 
{
}

void Rifleman::onCollisionEnd(CollisionEventArg* collision_event) 
{

}

float Rifleman::checkCollision(BaseObject * object, float dt)
{
	if (this->getStatus() == eStatus::DESTROY)
		return 0.0f;
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::BRIDGE || objectId == eID::LAND)
	{
		if (collisionBody->checkCollision(object, direction, dt))
		{
			if (direction == eDirection::TOP && this->getVelocity().y <= 0)
			{
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				auto movement = (Movement*)this->_listComponent["Movement"];
				movement->setVelocity(GVector2(movement->getVelocity().x, 0));
				gravity->setStatus(eGravityStatus::SHALLOWED);
			}
		}
	}
	else
	{
		collisionBody->checkCollision(object, dt);
	}
	return 0.0f;

}

void Rifleman::shoot() 
{
	float angle = getShootingAngle();
	auto pos = this->getPosition();

	if (this->isInStatus(AIMING_UP)) 
	{
		pos.x += this->getScale().x < 0 ? this->getSprite()->getFrameWidth() / 2 : -this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 2;
	}
	else if (this->isInStatus(AIMING_DOWN))
	{
		pos.x += this->getScale().x < 0 ? this->getSprite()->getFrameWidth() / 2 : -this->getSprite()->getFrameWidth() / 2;
		pos.y -= this->getSprite()->getFrameHeight() / 4;
	}
	else if (this->isInStatus(EXPOSING))
	{
		pos.x += this->getScale().x < 0 ? this->getSprite()->getFrameWidth() / 2 : -this->getSprite()->getFrameWidth() / 2;
		pos.y -= this->getSprite()->getFrameHeight() / 4.5;
		angle = this->getScale().x < 0 ? 90 : -90;
	}
	else if (this->isInStatus(NORMAL))
	{
		pos.x += this->getScale().x < 0 ? this->getSprite()->getFrameWidth() / 2 : -this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 4.5;
	}
	_listBullets.push_back(new Bullet(pos, (eBulletType)(ENEMY_BULLET | NORMAL_BULLET), angle)); // normalbullet ->hardcode
	_listBullets.back()->init();
}

void Rifleman::die() {
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	gravity->setStatus(eGravityStatus::SHALLOWED);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(0, 200));
}

void Rifleman::calculatingShootingDirection() 
{
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = (this->getPosition().x) - (bill->getPosition().x);
	if (dx >= 0)
		this->setScaleX(SCALE_FACTOR);
	else
		this->setScaleX(-SCALE_FACTOR);
}