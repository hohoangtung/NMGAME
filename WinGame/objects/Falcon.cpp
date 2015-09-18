#include "Falcon.h"


void Falcon::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::FALCON);
	_sprite->setFrameRect(0, 0, 32.0f, 32.0f);
	this->_sprite->setPosition(500, 200);
	GVector2 v(0, 0);
	GVector2 a(0, 0);
	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	_animation = new Animation(this->_sprite, 0.15f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_01"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_02"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_03"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_04"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_05"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_06"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::FALCON, "run_07"));
	_animation->setPosition(this->_sprite->getPosition());
	_stopwatch = new StopWatch();

}


void Falcon::draw(LPD3DXSPRITE spritehandle)
{
	this->_sprite->render(spritehandle, PlayScene::getViewport());
	_animation->draw(spritehandle, PlayScene::getViewport());
}

void Falcon::release()
{

}

IComponent* Falcon::getComponent(string componentName)
{
	return _listComponent.find(componentName)->second;
}

void Falcon::update(float deltatime)
{
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
	if (_stopwatch->isTimeLoop(8000.0f))
	{
		Movement* move = (Movement*)this->getComponent("Movement");
		GVector2 v = move->getVelocity();
		move->setVelocity(GVector2(-v.x, -v.y));
	}
	_animation->update(deltatime);
}