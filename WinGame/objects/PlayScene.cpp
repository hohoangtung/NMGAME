#include "PlayScene.h"


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
    
	//sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	//
	//auto soldier = new Soldier();
	//soldier->init();
	//_listobject.push_back(soldier);

	//auto falcon = new Falcon();
	//falcon->init();
	//_listobject.push_back(falcon);

	//auto mario = new Mario();
	//mario->init();
	//_listobject.push_back(mario);
	//_listControlObject.push_back(mario);

	//auto redcannon = new RedCannon(GVector2(500, 500));
	//_listobject.push_back(redcannon);

	auto bill = new Bill();
	bill->init();
	bill->setPosition(400, 200);

	_listControlObject.push_back(bill);
	_listobject.push_back(bill);

	_text = new Text(L"Arial", "", 10, 25);

	auto aircraft = new AirCraft(START_POSITION, HORIZONTAL_VELOC, AIRCRAFT_AMPLITUDE, AIRCRAFT_FREQUENCY, eAirCraftType::I);
	aircraft->init();
	_listobject.push_back(aircraft);
	_listControlObject.push_back(aircraft);

	auto soldier = new Soldier();
	soldier->init();
	_listobject.push_back(soldier);
	background = new Map();
	background->init("Resources//Map//map_index.txt");
	return true;
}

void PlayScene::updateInput(float dt)
{
	for each (IControlable* obj in _listControlObject)
	{
		obj->updateInput(dt);
	}
}

void PlayScene::update(float dt)
{

	//test sprite
	//sprite->setPositionX(sprite->getPosition().x + 1);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 1);
	//sprite->setScale(2);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 10);

	char str[100];
	sprintf(str, "delta time: %f", dt);
	_text->setText(str);

	for each (auto object in _listobject)
	{
		object->update(dt);
	}
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	//sprite->render(spriteHandle, _viewport);
	_text->draw();
	background->draw(spriteHandle);
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
