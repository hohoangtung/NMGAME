
#include"RedCannon.h"

int _shooting = 1;
#define PI 3.14159265
StopWatch *_loopwatch;

RedCannon::~RedCannon()
{
	for (auto it = _animation.begin(); it != _animation.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animation.clear();
	for (auto it = _listComponent.begin(); it != _listComponent.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_listComponent.clear();
	SAFE_DELETE(this->_sprite);
}
void RedCannon::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	this->_sprite->setPosition(500, 200);
	this->_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	/*this->setStatus(NORMAL);*/
	this->setScale(SCALE_FACTOR);

	/*GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));*/

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &RedCannon::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &RedCannon::onCollisionEnd);

	_animation[NORMAL] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[NORMAL]->addFrameRect(eID::REDCANNON, "run_01", "run_02", "run_03", NULL);

	_animation[NORMAL | SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[NORMAL | SHOOTING]->addFrameRect(eID::REDCANNON, "run_03", /*"run_02",*/ NULL);

	_animation[WT_LEFT_30] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_30]->addFrameRect(eID::REDCANNON, "run_04", "run_05", "run_06", NULL);

	_animation[WT_LEFT_30 | SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_30 | SHOOTING]->addFrameRect(eID::REDCANNON, "run_06", NULL);

	_animation[WT_LEFT_60] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_60]->addFrameRect(eID::REDCANNON, "run_07", "run_08", "run_09", NULL);

	_animation[WT_LEFT_60 | SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_60 | SHOOTING]->addFrameRect(eID::REDCANNON/*, "run_08"*/, "run_09", NULL);
	_animation[this->getStatus()]->stop();

	_stopwatch = new StopWatch();
	_loopwatch = new StopWatch();
	_explosion = NULL;
	this->addStatus(SHOOTING);
	this->setHitpoint(CANNON_HITPOINT);
	this->setScore(CANNON_SCORE);
	
}

void RedCannon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spriteHandle, viewport);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	this->_sprite->render(spriteHandle, viewport);
	_animation[this->getStatus()]->draw(spriteHandle, viewport);
	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->draw(spriteHandle, viewport);
	}
	
	
	
}


void RedCannon::update(float deltatime)
{

	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	this->getHitpoint();
	if (this->getHitpoint() == 0)
	{
		this->destroy();
		if (this->_stopwatch->isStopWatch(200))
		{
			auto pos = this->getPosition();
			_explosion = new Explosion(2);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(pos);
			this->setStatus(DESTROY);
			return;
		}

	}
	if (_loopwatch->isTimeLoop(2000.0f))
	{
		
		calculateBillangle();

	}
	
	if ((_billAngle>=-90 && _billAngle<-75))
	{
	this->setScale(SCALE_FACTOR);
	this->setStatus(NORMAL);
	_shootingAngle = -90;
	}
	else if (_billAngle >=-75 && _billAngle<-45)
	{
	this->setScale(SCALE_FACTOR);
	this->setStatus(WT_LEFT_30);
	_shootingAngle = -60;
	}
	else if(_billAngle>=-45 && _billAngle<=0)
	{
	this->setScale(SCALE_FACTOR);
	this->setStatus(WT_LEFT_60);
	_shootingAngle = -30;
	}

	if (_shooting == 1 && !this->isInStatus(SHOOTING))
	{
		this->addStatus(SHOOTING);
	}
	else if (this->isInStatus(SHOOTING))
	{
		this->removeStatus(SHOOTING);
	}

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->update(deltatime);
	}
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
	
	if (_stopwatch->isStopWatch(CANNON_SHOOTING_DELAY))
	{
		if (this->isInStatus(SHOOTING))
		{
			shoot();
		}
		_stopwatch->restart();
	}
	
	
	_animation[this->getStatus()]->update(deltatime);

}

void RedCannon::setShootingAngle(float angle)
{
	this->_shootingAngle= angle;
}

float RedCannon::getShootingAngle()
{
	return _shootingAngle;
}

void RedCannon::setBillAngle(float angle)
{
	this->_billAngle = angle;
}

float RedCannon::getBillAngle()
{
	return _billAngle;
}

IComponent* RedCannon::getComponent(string componentname)
{
	return _listComponent.find(componentname)->second;
}


void RedCannon::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	if (this->_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(this->_explosion);
	_listBullets.clear();
	_animation.clear();
	SAFE_DELETE(this->_sprite);
}

void RedCannon::onCollisionBegin(CollisionEventArg* collision_event)
{
	if (collision_event->_otherObject->getId() == eID::BULLET)
	{
		_hitpoint--;
	}
}
void RedCannon::onCollisionEnd(CollisionEventArg* collision_event)
{}

void RedCannon::setStatus(eStatus status)
{
	if (_status != status)
		_status = status;
}

void RedCannon::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}

void RedCannon::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}

bool RedCannon::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}
void RedCannon::shoot()
{

	float angle = getShootingAngle();
	auto pos = this->getPosition()  - GVector2(0, this->getSprite()->getFrameHeight() / 2);



	if (this->isInStatus(NORMAL))
	{

		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 2;
	}
	if (this->isInStatus(WT_LEFT_30))
	{

		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0;
	}
	if (this->isInStatus(WT_LEFT_60))
	{

		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0;
	}

	_listBullets.push_back(new Bullet(pos, angle));
	_listBullets.back()->init();
}
void RedCannon::calculateBillangle()
{
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = this->getPosition().x - bill->getPosition().x;
	float dy = this->getPosition().y - (bill->getPosition().y + bill->getSprite()->getFrameHeight() / 2);

	if (dx > 0 && dy < 0)
		_billAngle = -atan(dx / abs(dy)) * 180 / PI;
	else if (dx<0 && dy<0)
		_billAngle = -30;
	else _billAngle = -90;
}
void RedCannon::destroy()
{

}
void RedCannon::drophitpoint()
{
	this->setHitpoint(this->getHitpoint() - 1);
	if (this->getHitpoint() <= 0)
		this->setStatus(eStatus::DYING);
}