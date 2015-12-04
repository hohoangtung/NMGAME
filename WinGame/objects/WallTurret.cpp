#include"WallTurret.h"
int _shooting1 = 1;
StopWatch* _loopwatch1;
#define PI 3.14159265

WallTurret::WallTurret(eStatus status, GVector2 position) :BaseEnemy(eID::WALL_TURRET)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET);
	_sprite->setFrameRect(0, 0, 32, 32);
	this->setPosition(position);
	this->setStatus(status);
}

WallTurret::WallTurret(eStatus status, float x, float y) :BaseEnemy(eID::WALL_TURRET)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET);
	_sprite->setFrameRect(0, 0, 32, 32);
	GVector2 pos(x, y);
	this->setPosition(pos);
	this->setStatus(status);

}
WallTurret::WallTurret(eWT_Status wtstatus, GVector2 position) :BaseEnemy(eID::WALL_TURRET)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET);
	_sprite->setFrameRect(0, 0, 32, 32);
	this->setPosition(position);
	this->setStatus(wtstatus);
}
WallTurret::WallTurret(eWT_Status wtstatus, float x, float y) :BaseEnemy(eID::WALL_TURRET)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET);
	_sprite->setFrameRect(0, 0, 32, 32);
	GVector2 pos(x, y);
	this->setPosition(pos);
	this->setStatus(wtstatus);
}
WallTurret::~WallTurret()
{

}
void WallTurret::init()
{
	
	this->setScale(SCALE_FACTOR);

	auto collisionBody = new CollisionBody(this);
	_listComponent["collisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &WallTurret::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &WallTurret::onCollisionEnd);

	_animation[WT_APPEAR] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_CLOSE] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);

	_animation[WT_NORMAL] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_NORMAL]->addFrameRect(eID::WALL_TURRET, "normal","left_up_01","left_up_02", NULL);
	_animation[WT_NORMAL | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_NORMAL | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_02", NULL);

	_animation[WT_LEFT_60] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_60]->addFrameRect(eID::WALL_TURRET, "left_up_03", "left_up_04", "left_up_05", NULL);
	_animation[WT_LEFT_60|	WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_60 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_05", NULL);

	_animation[WT_LEFT_30] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_30]->addFrameRect(eID::WALL_TURRET, "left_up_06", "left_up_07", "left_up_08", NULL);
	_animation[WT_LEFT_30 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_30 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_08", NULL);

	_animation[WT_UP] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_UP]->addFrameRect(eID::WALL_TURRET, "up","right_up_01", "right_up_02", NULL);
	_animation[WT_UP | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_UP | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_02", NULL);

	_animation[WT_RIGHT_30] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_30]->addFrameRect(eID::WALL_TURRET, "right_up_03", "right_up_04","right_up_05", NULL);
	_animation[WT_RIGHT_30 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_30 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_05", NULL);

	_animation[WT_RIGHT_60] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_60]->addFrameRect(eID::WALL_TURRET, "right_up_06", "right_up_07", "right_up_08", NULL);
	_animation[WT_RIGHT_60 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_60 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_08", NULL);

	_animation[WT_RIGHT] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT]->addFrameRect(eID::WALL_TURRET, "right", "right_down_01", "right_down_02", NULL);
	_animation[WT_RIGHT | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_02", NULL);

	_animation[WT_RIGHT_120] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_120]->addFrameRect(eID::WALL_TURRET, "right_down_03", "right_down_04", "right_down_05", NULL);
	_animation[WT_RIGHT_120 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_120 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_05", NULL);

	_animation[WT_RIGHT_150] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_150]->addFrameRect(eID::WALL_TURRET, "right_down_06", "right_down_07", "right_down_08", NULL);
	_animation[WT_RIGHT_150 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_150 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_08", NULL);

	_animation[WT_DOWN] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_DOWN]->addFrameRect(eID::WALL_TURRET, "down", "left_down_01", "left_down_02", NULL);
	_animation[WT_DOWN | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_DOWN | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_02", NULL);

	_animation[WT_LEFT_150] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_150]->addFrameRect(eID::WALL_TURRET, "left_down_03", "left_down_04", "left_down_05", NULL);
	_animation[WT_LEFT_150 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_150 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_05", NULL);

	_animation[WT_LEFT_120] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_120]->addFrameRect(eID::WALL_TURRET, "left_down_06", "left_down_07", "left_down_08", NULL);
	_animation[WT_LEFT_120 | WT_SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_120 | WT_SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_08", NULL);

	_explosion = NULL;
	_stopwatch = new StopWatch();
	_loopwatch1 = new StopWatch();
	this->setHitpoint(WALL_TURRET_HITPOINT);
	this->setScore(WALL_TURRET_SCORE);

}
void WallTurret::update(float deltatime)
{
	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->_wtstatus == eWT_Status::WT_APPEAR || this->_wtstatus == eWT_Status::WT_CLOSE)
	{
		if (this->_wallturret_inactived == NULL)
			initWallTurret_inactived();
		else updateWallTurret_inactived(deltatime);

		this->_billAngle = -90;
	}

	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getHitpoint() <= 0)
	{
	
		if (_explosion == NULL)
			initExplosion();
		else {
			updateExplision(deltatime);
			if (this->_explosion->getStatus() == eStatus::DESTROY)
			this->setStatus(DESTROY);
		}		
	}
	
		
	if (this->_wallturret_inactived->getStatus() == eStatus::WAITING)
	{
		
		if (_loopwatch1->isTimeLoop(2000.0f))
		{
			calculateBillangle();
		}
		if (_billAngle >= -105 && _billAngle < -75)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_NORMAL);
			_shootingAngle = -90;

		}
		else if (_billAngle >= -75 && _billAngle < -45)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_LEFT_60);
			_shootingAngle = -60;
		}
		else if (_billAngle >= -45 && _billAngle < -15)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_LEFT_30);
			_shootingAngle = -30;
		}
		else if (_billAngle >= -15 && _billAngle < 15)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_UP);
			_shootingAngle = 0;
		}
		else if (_billAngle >= 15 && _billAngle < 45)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_RIGHT_30);
			_shootingAngle = 30;
		}
		else if (_billAngle >= 45 && _billAngle < 75)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_RIGHT_60);
			_shootingAngle = 60;
		}
		else if (_billAngle >= 75 && _billAngle < 105)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_RIGHT);
			_shootingAngle = 90;
		}
		else if (_billAngle >= 105 && _billAngle < 135)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_RIGHT_120);
			_shootingAngle = 120;
		}
		else if (_billAngle >= 135 && _billAngle < 165)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_RIGHT_150);
			_shootingAngle = 150;
		}

		else if (_billAngle >= 165 || _billAngle < -165)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_DOWN);
			_shootingAngle = 180;
		}
		else if (_billAngle >= -165 && _billAngle < -135)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_LEFT_150);
			_shootingAngle = -150;
		}

		else if (_billAngle >= -135 && _billAngle < -105)
		{
			this->setScale(SCALE_FACTOR);
			this->setStatus(WT_LEFT_120);
			_shootingAngle = -120;
		}

		if (_shooting1 == 1 && !this->isInStatus(WT_SHOOTING))
		{
			this->addStatus(WT_SHOOTING);
		}
		else if (this->isInStatus(WT_SHOOTING))
		{
			this->removeStatus(WT_SHOOTING);
		}
	}
	
	for (auto it = _listBullet.begin(); it != _listBullet.end(); it++)
	{
		(*it)->update(deltatime);
	}
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}

	if (_stopwatch->isStopWatch(WALL_TURRET_SHOOTING_DELAY))
	{
		if (this->isInStatus(WT_SHOOTING))
		{
			shoot();
		}
			_stopwatch->restart();
		
	}
	_animation[this->getWT_Status()]->update(deltatime);
}
void WallTurret::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spritehandle, viewport);
	if (this->_wtstatus == eWT_Status::WT_APPEAR||this->_wtstatus==eWT_Status::WT_CLOSE)
	{
		if (_wallturret_inactived != NULL)
			_wallturret_inactived->draw(spritehandle, viewport);
	}

	if (this->getStatus() == eStatus::DESTROY)
		return;

	this->_sprite->render(spritehandle, viewport);
	_animation[this->getWT_Status()]->draw(spritehandle, viewport);
	for (auto it = _listBullet.begin(); it != _listBullet.end(); it++)
	{
		(*it)->draw(spritehandle, viewport);
	}

}
void WallTurret::release()
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
	for (auto item : _listBullet)
	{
		delete item;
	}
	_listBullet.clear();

	if (_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(_explosion);
	SAFE_DELETE(this->_sprite);
}

IComponent* WallTurret::getComponent(string componentname)
{
	return _listComponent.find(componentname)->second;
}

void WallTurret::setStatus(eStatus status)
{
	if (_status != status)
		_status = status;
}
void WallTurret::setStatus(eWT_Status wtstatus)
{
	if (_wtstatus != wtstatus)
		_wtstatus = wtstatus;
}

eWT_Status WallTurret::getWT_Status()
{
	return this->_wtstatus;
}

void WallTurret::initWallTurret_inactived()
{
	if (_wtstatus == eWT_Status::WT_APPEAR)
	{

		_wallturret_inactived = new WallTurret_inactived(1, this->_sprite->getPosition());
		_wallturret_inactived->init();
	}
	if (_wtstatus == eWT_Status::WT_CLOSE)
	{
		_wallturret_inactived = new WallTurret_inactived(2, this->_sprite->getPosition());
		_wallturret_inactived->init();
	}
	
}
void WallTurret::updateWallTurret_inactived(float deltatime)
{
	_wallturret_inactived->update(deltatime);
}
void WallTurret::initExplosion()
{
	_explosion = new Explosion(2);
	_explosion->init();
	_explosion->setScale(SCALE_FACTOR);
	((Explosion*)_explosion)->setPosition(this->_sprite->getPosition());
}
void WallTurret::updateExplision(float deltatime)
{
	_explosion->update(deltatime);
}
void WallTurret::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}
void WallTurret::addStatus(eWT_Status wtstatus)
{
	this->setStatus(eWT_Status(this->getWT_Status() | wtstatus));
}

void WallTurret::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getWT_Status() & ~status));
}
void WallTurret::removeStatus(eWT_Status wtstatus)
{
	this->setStatus(eWT_Status(this->getWT_Status() & ~wtstatus));
}

bool WallTurret::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}
bool WallTurret::isInStatus(eWT_Status wtstatus)
{
	return (this->getWT_Status() & wtstatus) == wtstatus;
}

void WallTurret::setBillAngle(float angle)
{
	this->_billAngle = angle;
}

float WallTurret::getBillAngle()
{
	return _billAngle;
}
void WallTurret::setShootingAngle(float angle)
{
	this->_shootingAngle = angle;
}

float WallTurret::getShootingAngle()
{
	return _shootingAngle;
}

void WallTurret::onCollisionBegin(CollisionEventArg* collision_event)
{
	
}
void WallTurret::onCollisionEnd(CollisionEventArg* collision_event)
{

}

void WallTurret::shoot()
{
	float angle = this->getShootingAngle();
	float mx = this->getPosition().x;
	float my = this->getPosition().y;
	
	auto pos = this->getPosition() - GVector2(0, this->getSprite()->getFrameHeight() );
	
	if (this->isInStatus(WT_NORMAL))
	{
		pos.y += this->getSprite()->getFrameHeight() ;
	}
	
	_listBullet.push_back(new Bullet( pos, (eBulletType)(ENEMY_BULLET|NORMAL_BULLET), angle));
	_listBullet.back()->init();
}
void WallTurret::calculateBillangle()
{
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	float dx = this->getPosition().x - bill->getPosition().x;
	
	float dy = this->getPosition().y - (bill->getPosition().y + bill->getSprite()->getFrameHeight() / 2);
	if (dx > 0 && dy < 0)
		_billAngle = -atan(dx / (abs(dy))) * 180 / PI;
	else if (dx < 0 && dy < 0)
		_billAngle = atan(abs(dx) / abs(dy)) * 180 / PI;
	else if (dx>0 && dy>0)
		_billAngle = atan(dx / dy) * 180 / PI - 180;
	else if (dx<0 && dy>0)
		_billAngle = -atan(abs(dx) / dy) * 180 / PI + 180;
}
