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
	sprite->setPosition(sprite->getPosition().x + 1, sprite->getPosition().y);
	sprite->next();

	if (sprite->getPosition().x > 200)
	{
		SceneManager::getInstance()->replaceScene(new PlayScene());
	}
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	sprite->render(spriteHandle);
}

void PlayScene::release()
{
}
