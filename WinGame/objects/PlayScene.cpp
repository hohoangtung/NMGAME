#include "PlayScene.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

bool PlayScene::init()
{
	sprite = SpriteManager::getInstance()->getSprite(eID::FLOWER);
	sprite->setPosition(GVector3(100, 100, 1));
	return true;
}

void PlayScene::updateInput(float dt)
{
}

void PlayScene::update(float dt)
{
	//test
	sprite->setPosition(sprite->getPosition().x + 1, sprite->getPosition().y + 1);
	sprite->next();

	//sprite->setRotate(sprite->getRotate() + 10);
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	sprite->render(spriteHandle, _viewport);
}

void PlayScene::release()
{
}
