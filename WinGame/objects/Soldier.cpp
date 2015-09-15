#include "Soldier.h"

void Soldier::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SOLDIER);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	this->_sprite->setPosition(700, 500);
	GVector2 v(-10, 0);
	GVector2 a(0, 0);
	this->_listComponent.push_back(new Movement(a, v, this->_sprite));
	this->_listComponent_.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	_animation = new Animation(this->_sprite, 0.2f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_03"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_02"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_01"));
	//_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_04"));
	//_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_05"));
	//_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_06"));
	//_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SOLDIER, "run_07"));
	_animation->setPosition(this->_sprite->getPosition());
	_stopwatch = new StopWatch();
}

void Soldier::draw(LPD3DXSPRITE spritehandle)
{
	this->_sprite->render(spritehandle, PlayScene::getViewport());
	_animation->draw(spritehandle, PlayScene::getViewport());
}

void Soldier::release()
{

}

IComponent* Soldier::getComponent(string componentName)
{
	return _listComponent_.find(componentName)->second;
}

void Soldier::update(float deltatime)
{
	for (auto component : _listComponent)
	{
		component->update(deltatime);
	}
	for (auto it : _listComponent_)
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