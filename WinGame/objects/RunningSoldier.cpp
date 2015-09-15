#include "RunningSoldier.h"

void RunningSoldier::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::RUNNINGSOLDIER);
	this->_sprite->setPosition(700, 500);
	GVector2 v(-10, 0);
	GVector2 a(0, 0);
	this->_listComponent.push_back(new Movement(a, v, this->_sprite));
	this->_listComponent_.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));

	stopwatch = new StopWatch();
}

void RunningSoldier::draw(LPD3DXSPRITE spritehandle)
{
	this->_sprite->render(spritehandle, PlayScene::getViewport());
}

void RunningSoldier::release()
{

}

IComponent* RunningSoldier::getComponent(string componentName)
{
	return _listComponent_.find(componentName)->second;
}

void RunningSoldier::update(float deltatime)
{
	for (auto component : _listComponent)
	{
		component->update(deltatime);
	}
	for (auto it : _listComponent_)
	{
		it.second->update(deltatime);
	}
	if (stopwatch->isTimeLoop(8000.0f))
	{
		Movement* move = (Movement*)this->getComponent("Movement");
		GVector2 v = move->getVelocity();
		move->setVelocity(GVector2(-v.x, -v.y));
	}
	_sprite->nextFrame();
}