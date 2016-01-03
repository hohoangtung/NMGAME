
#include"RedCannon.h"

#define PI 3.14159265
StopWatch *_loopwatch;
int bullet = 0;
float delay1;

RedCannon::RedCannon(eStatus status, GVector2 position) :BaseEnemy(eID::REDCANNON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	
	_sprite->setFrameRect(0, 0, 32, 32);
	this->setPosition(position);
	this->setStatus(status);
	this->setWTStatus(eWT_Status::WT_NORMAL);
}
RedCannon::RedCannon(eStatus status, float x, float y) :BaseEnemy(eID::REDCANNON)
{
	//un-use
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	_sprite->setFrameRect(0, 0, 32, 32);
	GVector2 pos(x, y);
	this->setPosition(pos);
	this->setStatus(status);
	this->setWTStatus(eWT_Status::WT_NORMAL);
}
RedCannon::RedCannon(eWT_Status wtstatus, GVector2 position) :BaseEnemy(eID::REDCANNON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	_sprite->setFrameRect(0, 0, 32, 32);
	this->setPosition(position);
	this->setWTStatus(wtstatus);
}
RedCannon::RedCannon(eWT_Status wtstatus, float x, float y) :BaseEnemy(eID::REDCANNON)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::REDCANNON);
	_sprite->setFrameRect(0, 0, 32, 32);
	GVector2 pos(x, y);
	this->setPosition(pos);
	this->setWTStatus(wtstatus);
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

	_animation[WT_APPEAR] = new Animation(_sprite, CANNON_APPEAR_SPEED);
	_animation[WT_APPEAR]->addFrameRect(eID::REDCANNON, "appear_01", "appear_02", "appear_03", "appear_04", "appear_05", "appear_06", NULL);
	_animation[WT_APPEAR]->setLoop(false);

	_animation[WT_CLOSE] = new Animation(_sprite, CANNON_APPEAR_SPEED);
	_animation[WT_CLOSE]->addFrameRect(eID::REDCANNON, "appear_06", "appear_05", "appear_04", "appear_03", "appear_02", "appear_01", NULL);
	_animation[WT_CLOSE]->setLoop(false);

	_animation[WT_NORMAL] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_NORMAL]->addFrameRect(eID::REDCANNON, "run_01", "run_02", "run_03", NULL);

	_animation[WT_NORMAL | WT_SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_NORMAL | WT_SHOOTING]->addFrameRect(eID::REDCANNON, "run_01", "run_02","run_03", NULL);

	_animation[WT_LEFT_60] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_60]->addFrameRect(eID::REDCANNON, "run_04", "run_05", "run_06", NULL);

	_animation[WT_LEFT_60 | WT_SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_60 | WT_SHOOTING]->addFrameRect(eID::REDCANNON, "run_04","run_05","run_06", NULL);

	_animation[WT_LEFT_30] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_30]->addFrameRect(eID::REDCANNON, "run_07", "run_08", "run_09", NULL);

	_animation[WT_LEFT_30 | WT_SHOOTING] = new Animation(_sprite, CANNON_ANIMATION_SPEED);
	_animation[WT_LEFT_30 | WT_SHOOTING]->addFrameRect(eID::REDCANNON,"run_07", "run_08", "run_09", NULL);
	
	_stopwatch = new StopWatch();
	_loopwatch = new StopWatch();
	_explosion = NULL;
	this->setHitpoint(CANNON_HITPOINT);
	this->setScore(CANNON_SCORE);
}

void RedCannon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spriteHandle, viewport);
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getHitpoint()>0 && this->getStatus()!=eStatus::WAITING )
	{
		this->_sprite->render(spriteHandle, viewport);
		_animation[this->getWT_Status()]->draw(spriteHandle, viewport);
	}
	//for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	//{
	//	(*it)->draw(spriteHandle, viewport);
	//}
		
}

void RedCannon::update(float deltatime)
{
	this->rangeattack();
	this->checkIfOutofScreen();
	if (this->getHitpoint()<=0)
	{
		if (_explosion == nullptr)
			initExplosion();
		else
		{
			updateExplosion(deltatime);
			if (_explosion->getStatus() == eStatus::DESTROY)
			{
				this->setStatus(eStatus::DESTROY);
			}
		}	
	}

	if (_animation[WT_CLOSE]->isAnimate() == false)
	{
		this->setStatus(eStatus::DESTROY);
	}
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (_animation[WT_APPEAR]->isAnimate() == true)
	{
		_billAngle = -90;
		
	}

	if (!_animation[WT_APPEAR]->isAnimate() && this->isRange() )
	{
		
		if (_loopwatch->isTimeLoop(2000.0f))
		{
			calculateBillangle();
		}
		if ((_billAngle >= -90 && _billAngle < -75))
		{
			this->setScale(SCALE_FACTOR);
			this->setWTStatus(WT_NORMAL);
			_shootingAngle = -90;
		}
		else if (_billAngle >= -75 && _billAngle < -45)
		{
			this->setScale(SCALE_FACTOR);
			this->setWTStatus(WT_LEFT_60);
			_shootingAngle = -60;
		}
		else if (_billAngle >= -45 && _billAngle < 0)
		{
			this->setScale(SCALE_FACTOR);
			this->setWTStatus(WT_LEFT_30);
			_shootingAngle = -30;
		}
		this->addStatus(WT_SHOOTING);
		
	}
	
		//for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
		//{
		//	(*it)->update(deltatime);
		//}
		for (auto it : _listComponent)
		{
			it.second->update(deltatime);
		}
		
		if (this->getWT_Status() != eWT_Status::WT_APPEAR && this->getWT_Status() != eWT_Status::WT_CLOSE)
		{
			if (bullet >= 3)
			{
				bullet = 0;
				delay1 = CANNON_SHOOTING_DELAY;
				this->removeStatus(SHOOTING);
			}
			if (_stopwatch->isTimeLoop(delay1))
			{
				this->addStatus(SHOOTING);
				shoot();
				bullet++;
				delay1 = 100.0f;
			}
		}
			
		_animation[this->getWT_Status()]->update(deltatime);
}

void RedCannon::initExplosion()
{
	this->_explosion = new Explosion(2);
	_explosion->init();
	_explosion->setScale(SCALE_FACTOR);
	_explosion->setPosition(this->_sprite->getPosition());
	
}
void RedCannon::updateExplosion(float deltatime)
{
	_explosion->update(deltatime);
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
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID)
	{
	case eID::BILL:
	{
		if (collision_event->_otherObject->isInStatus(eStatus::DYING) == false)
		{
			collision_event->_otherObject->setStatus(eStatus::DYING);
			((Bill*)collision_event->_otherObject)->die();
		}
		break;
	}
	default:
		break;
	}
}
void RedCannon::onCollisionEnd(CollisionEventArg* collision_event)
{}
float RedCannon::checkCollision(BaseObject* object,float dt)
{
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	if (objectId==eID::BILL)
	collisionBody->checkCollision(object, dt);
	return 0.0f;
}
eWT_Status RedCannon::getWT_Status()
{
	return this->_wtstatus;
}
void RedCannon::setStatus(eStatus status)
{
	if (_status != status)
		_status = status;
}
void RedCannon::setWTStatus(eWT_Status wtstatus)
{
	if (_wtstatus != wtstatus)
		_wtstatus = wtstatus;
}
void RedCannon::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}
void RedCannon::addStatus(eWT_Status wtstatus)
{
	this->setWTStatus(eWT_Status(this->getWT_Status()| wtstatus));
}

void RedCannon::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}
void RedCannon::removeStatus(eWT_Status wtstatus)
{
	this->setWTStatus(eWT_Status(this->getWT_Status() & ~wtstatus));
}
bool RedCannon::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}
bool RedCannon::isInStatus(eWT_Status wtstatus)
{
	return (this->getWT_Status() & wtstatus) == wtstatus;
}
void RedCannon::shoot()
{
	float angle = getShootingAngle();
	auto pos = this->getPosition()  - GVector2(0, this->getSprite()->getFrameHeight() / 2);

	if (this->isInStatus(WT_NORMAL))
	{
		pos.y += this->getSprite()->getFrameHeight() / 2;
	}
	BulletManager::insertBullet(new Bullet(pos, (eBulletType)(ENEMY_BULLET | NORMAL_BULLET), angle));
	/*_listBullets.push_back(new Bullet(pos, (eBulletType)(ENEMY_BULLET|NORMAL_BULLET), angle));
	_listBullets.back()->init();*/
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
void RedCannon::rangeattack()
{
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	RECT thisBound = this->getBounding();
	
	
	if (viewport->isContains(thisBound))
	{
		this->setWTStatus(eWT_Status::WT_APPEAR);
		this->setStatus(eStatus::HIDDEN);
	}
	if (screenBound.left>thisBound.left || screenBound.bottom>thisBound.bottom)
	{
		this->setWTStatus(eWT_Status::WT_CLOSE);
	}
	
}
bool RedCannon::isRange()
{
	
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	RECT thisBound = BaseObject::getBounding();
	
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = this->getPosition().x - bill->getPosition().x;
	float dy = this->getPosition().y - (bill->getPosition().y + bill->getSprite()->getFrameHeight() / 2);
	float r_cannon = sqrt(dx*dx + dy*dy);
	if (screenBound.left > thisBound.left || screenBound.bottom > thisBound.bottom)
	{
		if (r_cannon < (WINDOW_WIDTH / 2 - 10))
			return true;

		return false;
	}
	else return true;
}
void RedCannon::checkIfOutofScreen()
{
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	RECT thisBound = this->getBounding();
	GVector2 position = this->getPosition();
	
	GVector2 viewportposition = viewport->getPositionWorld();
	if (thisBound.right < screenBound.left || thisBound.top < viewportposition.y - WINDOW_HEIGHT)
	{
		this->setStatus(eStatus::DESTROY);
	}
}
void RedCannon::drophitpoint()
{
	this->setHitpoint(this->getHitpoint() - 1);
	if (this->getHitpoint() <= 0)
	{
		this->setStatus(eStatus::BURST);
		SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
	}
}
void RedCannon::drophitpoint(int damage)
{
	this->setHitpoint(this->getHitpoint() - damage);
	if (this->getHitpoint() <= 0)
	{
		this->setStatus(eStatus::BURST);
		SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
	}
}

