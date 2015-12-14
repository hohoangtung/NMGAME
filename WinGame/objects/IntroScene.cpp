
#include "IntroScene.h"

void IntroScene::updateInput(float deltatime)
{

	if (_input->isKeyDown(DIK_UPARROW))
	{
		_yellowfancon->setPosition(GVector2(64.0f, 320.0f));
	}
	else if (_input->isKeyDown(DIK_DOWNARROW))
	{
		_yellowfancon->setPosition(GVector2(64.0f, 352.0f));
	}
	else if (_input->isKeyDown(DIK_X))
	{
		if (_introtable->getPositionX() > 0)
		{
			return;
		}
		SoundManager::getInstance()->Play(eSoundId::DESTROY_BRIDGE);
		if (_yellowfancon->getPositionY() == 320)
		{
			_ok = 1;
			_blank->setPosition(GVector2(_yellowfancon->getPositionX() + 40, _yellowfancon->getPositionY()));
		}
		else if (_yellowfancon->getPositionY() == 352)
		{
			_ok = 2;
			_blank->setPosition(GVector2(_yellowfancon->getPositionX() + 40, _yellowfancon->getPositionY()));
		}
	}
}
bool IntroScene::init()
{
	
	_blank = SpriteManager::getInstance()->getSprite(eID::BLANK);
	_blank->setScale(SCALE_FACTOR);
	_blank->setOrigin(VECTOR2ZERO);

	_yellowfancon = SpriteManager::getInstance()->getSprite(eID::YELLOWFALCON);
	_yellowfancon->setPosition(GVector2(64.0f, 320.0f));
	_yellowfancon->setScale(SCALE_FACTOR);
	_yellowfancon->setOrigin(VECTOR2ZERO);

	_introtable = SpriteManager::getInstance()->getSprite(eID::MENU);
	_introtable->setPosition(GVector2(512.0f, 0));
	_introtable->setScale(SCALE_FACTOR);
	_introtable->setOrigin(GVector2(0.0f,1.0f));

	_movement = new Movement(VECTOR2ZERO, GVector2(-200.0f, 0.0f), _introtable);

	_flash = new StopWatch();
	_access = new StopWatch();

	_ok = 0;
	_draw = 0;
	_drawyellowFalcon = false;
	//__hook(&InputController::__eventkeyPressed, _input, &IntroScene::onKeyPressed);
	SoundManager::getInstance()->Play(eSoundId::INTRO);
	return true;
}

void IntroScene::onKeyPressed(KeyEventArg* key_event)
{

}
void IntroScene::update(float dt)
{
	_movement->update(dt);
	if (_introtable->getPositionX() < 0)
	{
		_movement->setVelocity(VECTOR2ZERO);
		_drawyellowFalcon = true;
	}
	if (_ok == 1)
	{
		if (_flash->isTimeLoop(120))
		{
			_draw = ~_draw;
		}
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BRIDGE) == false)
		{
			auto play = new PlayScene();
			play->init();
			SceneManager::getInstance()->addScene(play);
		}
	}
	else if (_ok == 2)
	{
		if (_flash->isTimeLoop(120))
		{
			_draw = ~_draw;
		}
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BRIDGE) == false)
		{
			auto play = new PlayScene();
			play->init();
			SceneManager::getInstance()->addScene(play);
		}
	}
}
void IntroScene::draw(LPD3DXSPRITE spriteHandle)
{
	_introtable->render(spriteHandle);
	if (_drawyellowFalcon == true)
	{
		_yellowfancon->render(spriteHandle);
	}
	if (_ok != 0 && _draw == false)
	{
		_blank->render(spriteHandle);
	}
}
void IntroScene::release()
{
	_blank->release();
	SAFE_DELETE(_blank);

	_yellowfancon->release();
	SAFE_DELETE(_yellowfancon);

	_introtable->release();
	SAFE_DELETE(_introtable);
	SAFE_DELETE(_movement);
	SAFE_DELETE(_access);
}

IntroScene::IntroScene()
{

}

IntroScene::~IntroScene()
{
}