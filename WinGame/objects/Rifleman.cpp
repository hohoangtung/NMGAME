#include "Rifleman.h"

int shooting;
StopWatch *loopWatch;
Rifleman::Rifleman() : BaseEnemy(eID::RIFLEMAN) {}
Rifleman::~Rifleman() {}
 
void Rifleman::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::RIFLEMAN);
	_sprite->setFrameRect(0, 0, 23, 38);
	this->setPosition(500, 300);
	this->setStatus(NORMAL);
	this->setScale(SCALE_FACTOR);

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Rifleman::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Rifleman::onCollisionEnd);

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

	_animations[HIDING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[HIDING]->addFrameRect(eID::RIFLEMAN, "hide_01", NULL);

	_animations[EXPOSING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[EXPOSING]->addFrameRect(eID::RIFLEMAN, "hide_01", "expose_01", "expose_02", NULL);

	_animations[EXPOSING | SHOOTING] = new Animation(_sprite, RIFLEMAN_ANIMATION_SPEED);
	_animations[EXPOSING | SHOOTING]->addFrameRect(eID::RIFLEMAN, "expose_02", NULL);

	_stopwatch = new StopWatch();
	loopWatch = new StopWatch();
	// this->addStatus(SHOOTING);
	this->setHitpoint(RIFLEMAN_HITPOINT);
	this->setScore(RIFLEMAN_SCORE);
}

void Rifleman::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	this->_sprite->render(spritehandle, viewport);
	_animations[this->getStatus()]->draw(spritehandle, viewport);

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->draw(spritehandle, viewport);
	}
}

void Rifleman::release()
{

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
	if (shooting == 1 && !this->isInStatus(SHOOTING))
		this->addStatus(SHOOTING);
	else if (this->isInStatus(SHOOTING))
		this->removeStatus(SHOOTING);

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->update(deltatime);
	}

	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}

	if (loopWatch->isTimeLoop(2000.0f))
	{
		this->setShootingAngle(rand() % 360 - 180);
		shooting = rand() % 2;
	}
	if (_stopwatch->isStopWatch(RIFLEMAN_SHOOTING_DELAY))
	{
		if (this->isInStatus(SHOOTING))
			shoot();
		_stopwatch->restart();
	}
	_animations[this->getStatus()]->update(deltatime);
}

void Rifleman::setStatus(eStatus status) 
{
	if (_status != status)
		_status = status;
}

float Rifleman::getShootingAngle() 
{
	return _shootingAngle;
}
void Rifleman::onCollisionBegin(CollisionEventArg* collision_event) 
{
	if (collision_event->_otherObject->getId() == eID::BULLET)
	{
		_hitpoint--;
	}
}

void Rifleman::onCollisionEnd(CollisionEventArg* collision_event) 
{

}

void Rifleman::shoot() 
{
	float angle = getShootingAngle();
	auto pos = this->getPosition();// +GVector2(0, this->getSprite()->getFrameHeight() / 2);

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
	else if (this->isInStatus(NORMAL))
	{
		pos.x += this->getScale().x < 0 ? this->getSprite()->getFrameWidth() / 2 : -this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 4.5;
	}

	_listBullets.push_back(new Bullet(pos, angle));
	_listBullets.back()->init();
}

void Rifleman::die() {

}