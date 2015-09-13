#include "PlayScene.h"

PlayScene::PlayScene()
{
	_viewport = new Viewport(0,600);
}

PlayScene::~PlayScene()
{
}

bool PlayScene::init()
{
	sprite = SpriteManager::getInstance()->getSprite(eID::FLOWER);
	sprite->setPosition(GVector3(400, 500, 1));
	//sprite->setOrigin(GVector2(1.0f, 1.0f));
	sprite->setVelocityY(-100);
	sprite->setAccelerateY(-10);

	//_listobject.push_back(new OBJECT());

	return true;
}

void PlayScene::updateInput(float dt)
{
	
}

void PlayScene::update(float dt)
{
	//test
	//sprite->setPosition(sprite->getPosition().x + 1, sprite->getPosition().y + 1);
	sprite->update(dt);

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
