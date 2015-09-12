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
	sprite->setPosition(GVector3(400, 500, 1));
	//sprite->setOrigin(GVector2(1.0f, 1.0f));
	sprite->setVelocityY(-100);
	sprite->setAccelerateY(-10);
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
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	sprite->render(spriteHandle, _viewport);
}

void PlayScene::release()
{
}
