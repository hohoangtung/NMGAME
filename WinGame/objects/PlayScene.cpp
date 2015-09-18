#include "PlayScene.h"
#include "../debug.h"
#include "../FrameWork/Animation.h"

#include "Mario.h"
#include "Bill.h"

Viewport* PlayScene::_viewport = new Viewport(0, WINDOW_HEIGHT);

PlayScene::PlayScene()
{
	//_viewport = new Viewport(0,600);
}

PlayScene::~PlayScene()
{
	delete _viewport;
	_viewport = nullptr;
}
void PlayScene::setViewport(Viewport * viewport)
{
	if (_viewport != viewport)
		_viewport = viewport;
}

Viewport * PlayScene::getViewport()
{
	return _viewport;
}

bool PlayScene::init()
{
	sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	_animation = new Animation(sprite, 0.1f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_01"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_02"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_03"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_04"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_05"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_06"));
	//sprite->setPosition(GVector3(100, 100, 1));
	//sprite->drawBounding(true);
	//sprite->setRotate(30);

	_text = new Text(L"Arial", "", 10, 25);

	auto bill = new Bill();
	_listobject.push_back(bill);
	
	for each (auto object in _listobject)
	{
		object->init();
	}

	_listobject.back()->setPosition(400, 200);

	return true;
}

void PlayScene::updateInput(float dt)
{
	for each (auto object in _listobject)
	{
		if (object->getId() == eID::BILL)
		{
			((Bill*)object)->updateInput(dt);
		}
	}
}

void PlayScene::update(float dt)
{
	//test
	//sprite->setPositionX(sprite->getPosition().x + 1);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 1);
	//sprite->setScale(2);

	//_animation->update(dt);

	char str[100];
	sprintf(str, "delta time: %f", dt);

	_text->setText(str);

	//sprite->nextFrame();
	__debugoutput(dt / 1000);

	//sprite->setRotate(sprite->getRotate() + 10);
	//_viewport->setPositionWorld(GVector2(_viewport->getPositionWorld().x + 2, _viewport->getPositionWorld().y));
	for each (auto object in _listobject)
	{
		object->update(dt);
	}
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	//sprite->render(spriteHandle, _viewport);
	_text->draw();
	//_animation->draw(spriteHandle, _viewport);

	for each (auto object in _listobject)
	{
		object->draw(spriteHandle, _viewport);
	}
}

void PlayScene::release()
{
	for each (auto object in _listobject)
	{
		object->release();
	}
}
