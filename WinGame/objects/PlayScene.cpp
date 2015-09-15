#include "PlayScene.h"
#include "../debug.h"
#include "../FrameWork/Animation.h"

#include "Mario.h"
#include "RunningSoldier.h"
Viewport* PlayScene::_viewport = new Viewport(0, 600);
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
	sprite->setFrameRect(7.0f, 56.0f, 51.0f, 43.0f);
	
	//_animation = new Animation(SpriteManager::getInstance()->getSprite(eID::BILL), 0.1f);
	//_animation->addFrameRect("", 169.0f, 134.0f, 186.0f, 167.0f);
	//_animation->addFrameRect("", 190.0f, 137.0f, 211.0f, 167.0f);
	//_animation->addFrameRect("", 217.0f, 132.0f, 237.0f, 167.0f);
	//_animation->addFrameRect("", 241.0f, 132.0f, 258.0f, 167.0f);
	//_animation->addFrameRect("", 145.0f, 135.0f, 163.0f, 167.0f);

	_animation = new Animation(sprite, 0.2f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_01"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_02"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_03"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_04"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_05"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_06"));
	_animation->createAnimationFromFile("Resources\\test_animation.txt");

	_animation->setPosition(GVector2(200, 100));

	//sprite->setPosition(GVector3(100, 100, 1));
	//sprite->drawBounding(true);
	//sprite->setRotate(30);

	//_listobject.push_back(new OBJECT());
    // auto mario = new Mario();
	// mario->init();
	// _listobject.push_back(mario);
	sprite = SpriteManager::getInstance()->getSprite(eID::RUNNINGSOLDIER);
	// sprite->setPosition(GVector3(400, 500, 1));
	// sprite->setOrigin(GVector2(1.0f, 1.0f));
	auto runningSoldier = new RunningSoldier();
	runningSoldier->init();
	_listobject.push_back(runningSoldier);
	_text = new Text(L"Arial", "", 10, 25);

	return true;
}

void PlayScene::updateInput(float dt)
{
	
}

void PlayScene::update(float dt)
{
	//test
	//sprite->setPositionX(sprite->getPosition().x + 1);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 1);
	//sprite->setScale(2);

	_animation->update(dt);

	char str[100];
	// sprintf(str, "delta time: %f", dt);

	//_text->setText(str);

	sprite->nextFrame();
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
	sprite->render(spriteHandle, _viewport);
	_text->draw();
	_animation->draw(spriteHandle, _viewport);

	for each (auto object in _listobject)
	{
		object->draw(spriteHandle);
	}
}

void PlayScene::release()
{
	for each (auto object in _listobject)
	{
		object->release();
	}
}
