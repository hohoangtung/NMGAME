#include "PlayScene.h"
#include "../debug.h"

#include "Mario.h"
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
	sprite = SpriteManager::getInstance()->getSprite(eID::FLOWER);
	sprite->setPosition(GVector3(400, 500, 1));
	//sprite->setOrigin(GVector2(1.0f, 1.0f));

	//_listobject.push_back(new OBJECT());
	auto mario = new Mario();
	mario->init();
	_listobject.push_back(mario);

	_text = new Text(L"Arial", "", 10, 25);

	return true;
}

void PlayScene::updateInput(float dt)
{
	
}

void PlayScene::update(float dt)
{
	//test
	//sprite->setPosition(sprite->getPosition().x + 1, sprite->getPosition().y + 1);
	//sprite->nextFrame();

	//char str[100];
	//sprintf(str, "fps: %f", dt);

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
