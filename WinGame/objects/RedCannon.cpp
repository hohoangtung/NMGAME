
#include"RedCannon.h"

int _shooting = 1;
#define PI 3.14159265
StopWatch *_loopwatch;

RedCannon::RedCannon(eStatus status, GVector2 position) :BaseEnemy(eID::REDCANNON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	
	_sprite->setFrameRect(0, 0, 32, 32);
	this->setPosition(position);
	this->setStatus(status);
}
RedCannon::RedCannon(eStatus status, float x, float y) :BaseEnemy(eID::REDCANNON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	_sprite->setFrameRect(0, 0, 32, 32);
	GVector2 pos(x, y);
	this->setPosition(pos);
	this->setStatus(status);
}
RedCannon::~RedCannon()
{

}
void RedCannon::init()
{
	
	
	this->setScale(SCALE_FACTOR);

	

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
	/*_explosion = NULL;*/
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
	//for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	//{
	//	(*it)->draw(spriteHandle, viewport);
	//}
	//
	
	
}


void RedCannon::update(float deltatime)
{

	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	this->getHitpoint();
	if (this->getHitpoint() <= 0)
	{
		
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
		this->setStatus(NORMAL);
		_shootingAngle = -90.0f;
	}
	else if (_billAngle >=-75 && _billAngle<-45)
	{
		this->setStatus(WT_LEFT_30);
		_shootingAngle = -60.0f;
	}
	else if(_billAngle>=-45 && _billAngle<=0)
	{
		this->setStatus(WT_LEFT_60);
		_shootingAngle = -30.0f;
	}

	if (_shooting == 1 && !this->isInStatus(SHOOTING))
	{
		this->addStatus(SHOOTING);
	}
	else if (this->isInStatus(SHOOTING))
	{
		this->removeStatus(SHOOTING);
	}

	//for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	//{
	//	(*it)->update(deltatime);
	//}
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
	for (auto ani : _animation)
	{
		delete ani.second;
	}
	_animation.clear();
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	//for (auto item : _listBullets)
	//{
	//	delete item;
	//}
	//_listBullets.clear();
	if (_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(_explosion);
	SAFE_DELETE(this->_sprite);
}

void RedCannon::onCollisionBegin(CollisionEventArg* collision_event)
{
	
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
	
	BulletManager::insertBullet(new Bullet(pos, (eBulletType)(ENEMY_BULLET | NORMAL_BULLET), angle));
	//_listBullets.push_back(new Bullet(pos, (eBulletType)(ENEMY_BULLET|NORMAL_BULLET), angle));
	//_listBullets.back()->init();
}
void RedCannon::calculateBillangle()
{
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = this->getPosition().x - bill->getPosition().x;
	float dy = this->getPosition().y - (bill->getPosition().y + bill->getSprite()->getFrameHeight() / 2);

	if (dx > 0 && dy < 0)
		_billAngle = D3DXToDegree(-atan(dx / -dy));
		//_billAngle = -atan(dx / abs(dy)) * 180 / PI;
	else if (dx<0 && dy<0)
		_billAngle = -30;
	else _billAngle = -90;
}

void RedCannon::drophitpoint()
{
	this->setHitpoint(this->getHitpoint() - 1);
	if (this->getHitpoint() <= 0)
	{
		this->setStatus(eStatus::DYING);
		SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
	}
}
void RedCannon::drophitpoint(int damage)
{
	this->setHitpoint(this->getHitpoint() - damage);
	if (this->getHitpoint() <= 0)
	{
		this->setStatus(eStatus::DYING);
		SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
	}
}


