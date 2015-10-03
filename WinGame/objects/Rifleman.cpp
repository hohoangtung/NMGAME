#include "Rifleman.h"

int shooting;

void Rifleman::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::RIFLEMAN);
	_sprite->setFrameRect(0, 0, 23, 38);
	_sprite->setPosition(500, 400);
	this->setStatus(NORMAL);

	_animations[NORMAL] = new Animation(_sprite, 0.15f);
	_animations[NORMAL]->addFrameRect(eID::RIFLEMAN, "normal_01", NULL);

	_animations[SHOOTING | NORMAL] = new Animation(_sprite, 0.15f);
	_animations[SHOOTING | NORMAL]->addFrameRect(eID::RIFLEMAN, "normal_01", "shoot_01", NULL);

	_animations[AIMING_UP] = new Animation(_sprite, 0.15f);
	_animations[AIMING_UP]->addFrameRect(eID::RIFLEMAN, "aim_up_01", NULL);

	_animations[AIMING_DOWN] = new Animation(_sprite, 0.15f);
	_animations[AIMING_DOWN]->addFrameRect(eID::RIFLEMAN, "aim_down_01", NULL);

	_animations[AIMING_UP | SHOOTING] = new Animation(_sprite, 0.15f);
	_animations[AIMING_UP | SHOOTING]->addFrameRect(eID::RIFLEMAN, "aim_up_01", "shoot_up_01", NULL);

	_animations[AIMING_DOWN | SHOOTING] = new Animation(_sprite, 0.15f);
	_animations[AIMING_DOWN | SHOOTING]->addFrameRect(eID::RIFLEMAN, "aim_down_01", NULL);

	_animations[HIDING] = new Animation(_sprite, 0.15f);
	_animations[HIDING]->addFrameRect(eID::RIFLEMAN, "hide_01", NULL);

	_animations[EXPOSING] = new Animation(_sprite, 0.15f);
	_animations[EXPOSING]->addFrameRect(eID::RIFLEMAN, "hide_01", "expose_01", "expose_02", NULL);

	_animations[EXPOSING | SHOOTING] = new Animation(_sprite, 0.15f);
	_animations[EXPOSING | SHOOTING]->addFrameRect(eID::RIFLEMAN, "expose_02", NULL);

	_stopwatch = new StopWatch();
	// this->addStatus(SHOOTING);
}

void Rifleman::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	this->_sprite->render(spritehandle, viewport);
	_animations[this->getStatus()]->draw(spritehandle, viewport);
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
	switch (this->getStatus())
	{
	case NORMAL: case AIMING_UP: case AIMING_DOWN:
		if ((_shootingAngle >= 330 && _shootingAngle < 360) || (_shootingAngle >=0 && _shootingAngle < 30)) {
			this->_sprite->setScaleX(-1);
			this->setStatus(NORMAL);
		}
		else if (_shootingAngle >= 30 && _shootingAngle < 90) {
			this->_sprite->setScaleX(-1);
			this->setStatus(AIMING_UP);
		}
		else if (_shootingAngle >= 90 && _shootingAngle < 150) {
			this->_sprite->setScaleX(1);
			this->setStatus(AIMING_UP);
		}
		else if (_shootingAngle >= 150 && _shootingAngle < 210) {
			this->_sprite->setScaleX(1);
			this->setStatus(NORMAL);
		}
		else if (_shootingAngle >= 210 && _shootingAngle < 270) {
			this->_sprite->setScaleX(1);
			this->setStatus(AIMING_DOWN);
		}
		else if (_shootingAngle >= 270 && _shootingAngle < 330) {
			this->_sprite->setScaleX(-1);
			this->setStatus(AIMING_DOWN);
		}

		break;
	}
	if (shooting == 1)
		this->addStatus(SHOOTING);
	else
		this->removeStatus(SHOOTING);
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
	if (_stopwatch->isTimeLoop(2000.0f))
	{
		this->setShootingAngle(rand() % 360);
		shooting = rand() % 2;
	}
	_animations[this->getStatus()]->update(deltatime);
}