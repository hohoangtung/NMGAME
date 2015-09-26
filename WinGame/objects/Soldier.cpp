#include "Soldier.h"

bool jumped = false;
void Soldier::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SOLDIER);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	this->_sprite->setPosition(600, 500);
	// this->setPosition(100, 100);
	GVector2 v(-20, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Gravity", new Gravity(GVector2(0, -120), (Movement*)(this->getComponent("Movement")))));
	Animation *runningAnimation = new Animation(this->_sprite, 0.15f);
	//_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_07"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_01"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_02"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_03"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_04"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_05"));
	runningAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_06"));
	// runningAnimation->setPosition(this->_sprite->getPosition());
	Animation *jumpingAnimation = new Animation(this->_sprite, 0.15f);
	jumpingAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "jump_01"));
	Animation *shootingAnimation = new Animation(this->_sprite, 0.15f);
	shootingAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "shoot_01"));
	shootingAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "shoot_02"));
	Animation *dyingAnimation = new Animation(this->_sprite, 0.15f);
	dyingAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "die_01"));
	Animation *lyingAnimation = new Animation(this->_sprite, 0.15f);
	lyingAnimation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "lie_01"));

	this->_animations.insert(pair<eStatus, Animation*>(RUNNING, runningAnimation));
	this->_animations.insert(pair<eStatus, Animation*>(JUMPING, jumpingAnimation));
	this->_animations.insert(pair<eStatus, Animation*>(SHOOTING, shootingAnimation));

	_stopwatch = new StopWatch();
}

void Soldier::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	this->_sprite->render(spritehandle, PlayScene::getViewport());
	_animations[this->getStatus()]->draw(spritehandle, viewport);
}

void Soldier::release()
{

}

IComponent* Soldier::getComponent(string componentName)
{
	return _listComponent.find(componentName)->second;
}

void Soldier::update(float deltatime)
{
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	Movement *movement = (Movement*)this->getComponent("Movement");
	switch (this->getStatus())
	{
	case RUNNING:
		movement->setVelocity(GVector2(-30, 0));
		gravity->setStatus(SHALLOWED);
		if (this->_sprite->getPositionX() < 500 && !jumped){
			jumped = true;
			movement->setVelocity(GVector2(movement->getVelocity().x, 60));
			this->setStatus(JUMPING);
		}
		break;
	case JUMPING:
		gravity->setStatus(FALLING__DOWN);
		if (this->_sprite->getPositionY() < 400) {
			this->_sprite->setPositionY(400);
			this->setStatus(RUNNING);
		}
		break;
	//case FALLING:
	//	break;
	case SHOOTING:
		gravity->setStatus(SHALLOWED);
		movement->setVelocity(GVector2(0, 0));
		break;
	case LAYING_DOWN:
		gravity->setStatus(SHALLOWED);
		movement->setVelocity(GVector2(0, 0));
		break;
	//case DYING:
	//	gravity->setStatus(FALLING__DOWN);
	//	movement->setVelocity(GVector2(0, 60));
	//	break;
	}
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
	//if (_stopwatch->isTimeLoop(8000.0f))
	//{
	//	Movement *move = (Movement*)this->getComponent("Movement");
	//	GVector2 v = move->getVelocity();
	//	move->setVelocity(GVector2(-v.x, -v.y));
	//}
	_animations[this->getStatus()]->update(deltatime);
}