#include "SelectionScene.h"

SelectionScene::SelectionScene()
{
}

SelectionScene::~SelectionScene()
{
}

bool SelectionScene::init()
{
	_gap = 64;
	_maxIndex = 1;

	_stage1 = new TextSprite(eID::FONTFULL, "STAGE 1: JUNGLE", GVector2(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4));
	_stage1->init();
	_stage1->setScale(SCALE_FACTOR);
	_stage1->setOrigin(VECTOR2ZERO);
	_stage1->setOpacity(0.8f);

	_stage3 = new TextSprite(eID::FONTFULL, "STAGE 3: WATERFALL", GVector2(WINDOW_WIDTH / 4, _stage1->getPosition().y + _gap));
	_stage3->init();
	_stage3->setScale(SCALE_FACTOR);
	_stage3->setOrigin(VECTOR2ZERO);
	_stage3->setOpacity(0.8f);

	_yellowFancon = SpriteManager::getInstance()->getSprite(eID::YELLOWFALCON);
	_yellowFancon->setPosition(GVector2(32.0f, _stage1->getPosition().y));
	_yellowFancon->setScale(SCALE_FACTOR);
	_yellowFancon->setOrigin(VECTOR2ZERO);

	_tutLayer = SpriteManager::getInstance()->getSprite(eID::TUT_LAYER);
	_tutLayer->setPosition(0, WINDOW_HEIGHT);
	_tutLayer->setOrigin(VECTOR2ZERO);
	_tutLayer->setOpacity(0.7f);

	return true;
}

void SelectionScene::updateInput(float dt)
{
	if(_input->isKeyDown(DIK_UPARROW))
	{
		if (_selectIndex > 0)
			_selectIndex--;
	}
	else if (_input->isKeyDown(DIK_DOWNARROW))
	{
		if (_selectIndex < _maxIndex)
			_selectIndex++;
	}
	else if (_input->isKeyDown(DIK_X))
	{
		SoundManager::getInstance()->Play(eSoundId::DESTROY_BRIDGE);

		if (_selectIndex == 0)
		{
			auto play = new BeginPlayScene(0, 3, 1);
			SceneManager::getInstance()->replaceScene(play);
		}
		else if (_selectIndex == 1)
		{
			auto play = new BeginPlayScene(0, 3, 3);
			SceneManager::getInstance()->replaceScene(play);
		}
	}
}

void SelectionScene::update(float dt)
{
	_yellowFancon->setPositionY(_stage1->getPosition().y + _gap * _selectIndex);
}

void SelectionScene::draw(LPD3DXSPRITE spriteHandle)
{
	_tutLayer->render(spriteHandle);
	_yellowFancon->render(spriteHandle);

	_stage1->draw(spriteHandle);
	_stage3->draw(spriteHandle);
}

void SelectionScene::release()
{
	SAFE_DELETE(_yellowFancon);
	SAFE_DELETE(_stage1);
	SAFE_DELETE(_stage1);
}