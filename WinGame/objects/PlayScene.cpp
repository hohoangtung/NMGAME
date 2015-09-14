#include "PlayScene.h"
#include "../debug.h"

PlayScene::PlayScene()
{
	_viewport = new Viewport(0,600);
}

PlayScene::~PlayScene()
{
}

bool PlayScene::init()
{
	sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	sprite->setFrameRect(7.0f, 56.0f, 51.0f, 43.0f);
	
	sprite->setPosition(GVector3(100, 100, 1));
	//sprite->setOrigin(GVector2(1.0f, 1.0f));

	//_listobject.push_back(new OBJECT());

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

	char str[100];
	sprintf(str, "fps: %f", dt);

	_text->setText(str);

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
