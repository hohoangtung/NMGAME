
#include "GameOverScene.h"
#include "IntroScene.h"

GameOverScene::GameOverScene(int score, int prePlayScene)
{
	this->_score = score;
	this->_prePlayScene = prePlayScene;
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::updateInput(float deltatime)
{
	if (_input->isKeyDown(DIK_UP))
	{
		_index = 0;
	}
	else if (_input->isKeyDown(DIK_DOWN))
	{
		_index = 1;
	}
	else if (_input->isKeyDown(DIK_X))
	{
		if (_index == 0)
		{
			Scene* scene = nullptr;
			// Vô scene play
			switch (_prePlayScene)
			{
			case 1:
				scene = new PlayScene();
				SceneManager::getInstance()->replaceScene(scene);
				return;
			default:
				break;
			}
		}
		if (_index == 1)
		{
			// Vô scene menu
			auto menu = new IntroScene();
			SceneManager::getInstance()->replaceScene(menu);
			return;
		}
	}
}
bool GameOverScene::init()
{
	_index = 0;
	_blank = SpriteManager::getInstance()->getSprite(eID::BLANK);
	_blank->setScale(SCALE_FACTOR);
	_blank->setOrigin(VECTOR2ZERO);

	_yellowfancon = SpriteManager::getInstance()->getSprite(eID::YELLOWFALCON);
	_yellowfancon->setPosition(GVector2(192.0f, 320.0f));		// hardcode
	_yellowfancon->setScale(SCALE_FACTOR);
	_yellowfancon->setOrigin(VECTOR2ZERO);

	_background = SpriteManager::getInstance()->getSprite(eID::GAME_OVER_SCENE);
	_background->setPosition(VECTOR2ZERO);
	_background->setScale(SCALE_FACTOR);
	_background->setOrigin(GVector2(0.0f, 1.0f));


	_flash = new StopWatch();

	return true;
}

void GameOverScene::onKeyPressed(KeyEventArg* key_event)
{

}
void GameOverScene::update(float dt)
{
	if (_index == 0)
	{
		_yellowfancon->setPositionY(320.0f);
	}
	if (_index == 1)
	{
		_yellowfancon->setPositionY(350.0f);
	}
}
void GameOverScene::draw(LPD3DXSPRITE spriteHandle)
{
	_background->render(spriteHandle);
	_yellowfancon->render(spriteHandle);
}
void GameOverScene::release()
{
	SAFE_DELETE(_blank);
	SAFE_DELETE(_yellowfancon);
	SAFE_DELETE(_background);

}
