#include"WallTurret.h"
int _shooting1 = 1;
StopWatch* _loopwatch1;
#define PI 3.14159265

WallTurret::~WallTurret()
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
void WallTurret::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::WALL_TURRET);
	this->_sprite->setPosition(600, 200);
	this->_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	this->setStatus(NORMAL);
	this->setScale(SCALE_FACTOR);

	auto collisionBody = new CollisionBody(this);
	_listComponent["collisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &WallTurret::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &WallTurret::onCollisionEnd);

	_animation[NORMAL] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[NORMAL]->addFrameRect(eID::WALL_TURRET, "normal","left_up_01","left_up_02", NULL);
	_animation[NORMAL | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[NORMAL | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_02", NULL);

	_animation[WT_LEFT_60] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_60]->addFrameRect(eID::WALL_TURRET, "left_up_03", "left_up_04", "left_up_05", NULL);
	_animation[WT_LEFT_60|SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_60 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_05", NULL);

	_animation[WT_LEFT_30] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_30]->addFrameRect(eID::WALL_TURRET, "left_up_06", "left_up_07", "left_up_08", NULL);
	_animation[WT_LEFT_30 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_30 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_up_08", NULL);

	_animation[WT_UP] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_UP]->addFrameRect(eID::WALL_TURRET, "up","right_up_01", "right_up_02", NULL);
	_animation[WT_UP | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_UP | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_02", NULL);

	_animation[WT_RIGHT_30] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_30]->addFrameRect(eID::WALL_TURRET, "right_up_03", "right_up_04","right_up_05", NULL);
	_animation[WT_RIGHT_30 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_30 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_05", NULL);

	_animation[WT_RIGHT_60] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_60]->addFrameRect(eID::WALL_TURRET, "right_up_06", "right_up_07", "right_up_08", NULL);
	_animation[WT_RIGHT_60 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_60 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_up_08", NULL);

	_animation[WT_RIGHT] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT]->addFrameRect(eID::WALL_TURRET, "right", "right_down_01", "right_down_02", NULL);
	_animation[WT_RIGHT | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_02", NULL);

	_animation[WT_RIGHT_120] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_120]->addFrameRect(eID::WALL_TURRET, "right_down_03", "right_down_04", "right_down_05", NULL);
	_animation[WT_RIGHT_120 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_120 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_05", NULL);

	_animation[WT_RIGHT_150] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_150]->addFrameRect(eID::WALL_TURRET, "right_down_06", "right_down_07", "right_down_08", NULL);
	_animation[WT_RIGHT_150 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_RIGHT_150 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "right_down_08", NULL);

	_animation[WT_DOWN] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_DOWN]->addFrameRect(eID::WALL_TURRET, "down", "left_down_01", "left_down_02", NULL);
	_animation[WT_DOWN | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_DOWN | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_02", NULL);

	_animation[WT_LEFT_150] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_150]->addFrameRect(eID::WALL_TURRET, "left_down_03", "left_down_04", "left_down_05", NULL);
	_animation[WT_LEFT_150 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_150 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_05", NULL);

	_animation[WT_LEFT_120] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_120]->addFrameRect(eID::WALL_TURRET, "left_down_06", "left_down_07", "left_down_08", NULL);
	_animation[WT_LEFT_120 | SHOOTING] = new Animation(_sprite, WALL_TURRET_ANIMATION_SPEED);
	_animation[WT_LEFT_120 | SHOOTING]->addFrameRect(eID::WALL_TURRET, "left_down_08", NULL);

	_explosion = NULL;
	_stopwatch = new StopWatch();
	_loopwatch1 = new StopWatch();
	this->setHitpoint(WALL_TURRET_HITPOINT);
	this->setScore(WALL_TURRET_SCORE);
	this->addStatus(SHOOTING);
}
void WallTurret::update(float deltatime)
{
	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (this->getStatus() == eStatus::DESTROY)
		return;
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

	if (_loopwatch1->isTimeLoop(2000.0f))
	{

		calculateBillangle();
	}
	

	if (_billAngle >= -105 && _billAngle < -75)
	{
		this->setScale(SCALE_FACTOR);
		this->setStatus(NORMAL);
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
	else if (_billAngle >= 165 || _billAngle<-165)
	{
		this->setScale(SCALE_FACTOR);
		this->setStatus(WT_DOWN);
		_shootingAngle = 180;
	}
	else if (_billAngle >= -165 && _billAngle <-135)
	{
		this->setScale(SCALE_FACTOR);
		this->setStatus(WT_LEFT_150);
		_shootingAngle = -150;
	}
	else if (_billAngle >= -135 && _billAngle<-105)
	{
		this->setScale(SCALE_FACTOR);
		this->setStatus(WT_LEFT_120);
		_shootingAngle = -120;
	}

	if (_shooting1 == 1 && !this->isInStatus(SHOOTING))
	{
		this->addStatus(SHOOTING);
	}
	else if (this->isInStatus(SHOOTING))
	{
		this->removeStatus(SHOOTING);
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
		if (this->isInStatus(SHOOTING))
		{
			shoot();
		}
			_stopwatch->restart();
		
	}
	_animation[this->getStatus()]->update(deltatime);
}
void WallTurret::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (_explosion != NULL)
		_explosion->draw(spritehandle, viewport);
	if (this->getStatus() == eStatus::DESTROY)
		return;

	this->_sprite->render(spritehandle, viewport);
	_animation[this->getStatus()]->draw(spritehandle, viewport);
	for (auto it = _listBullet.begin(); it != _listBullet.end(); it++)
	{
		(*it)->draw(spritehandle, viewport);
	}

}
void WallTurret::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	if (_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(_explosion);
	SAFE_DELETE(this->_sprite);
	_listBullet.clear();
	_animation.clear();
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

void WallTurret::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}

void WallTurret::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}

bool WallTurret::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
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
	if (collision_event->_otherObject->getId() == eID::BULLET)
	{
		_hitpoint--;
	}
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
	
	if (this->isInStatus(NORMAL))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() ;
	}
	if (this->isInStatus(WT_LEFT_60))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0;
	}
	if (this->isInStatus(WT_LEFT_30))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_UP))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 2;
	}
	if (this->isInStatus(WT_RIGHT_30))
	{
		pos.x += this->getScale().x < 0 == -this->getSprite()->getFrameWidth() / 2;
		pos.y -=  this->getSprite()->getFrameHeight() / 2;
	}
	if (this->isInStatus(WT_RIGHT_30))
	{
		pos.x += this->getScale().x < 0 == -this->getSprite()->getFrameWidth() / 2;
		pos.y += this->getSprite()->getFrameHeight() / 2;
	}
	if (this->isInStatus(WT_RIGHT))
	{
		pos.x += this->getScale().x < 0 == -this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_RIGHT_120))
	{
		pos.x += this->getScale().x < 0 == -this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_RIGHT_150))
	{
		pos.x += this->getScale().x < 0 == -this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_DOWN))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_LEFT_150))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0; 
	}
	if (this->isInStatus(WT_LEFT_120))
	{
		pos.x += this->getScale().x < 0 == this->getSprite()->getFrameWidth() / 2;
		pos.y += 0;
	}
	_listBullet.push_back(new Bullet( pos, angle));
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
void WallTurret::destroy()
{}