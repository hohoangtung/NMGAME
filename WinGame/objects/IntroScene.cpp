
#include "IntroScene.h"

void IntroScene::updateInput(float deltatime)
{

	if (_input->isKeyDown(DIK_UPARROW))
	{
		_yellowfancon->setPosition(GVector2(32.0f, 324.0f));
	}
	else if (_input->isKeyDown(DIK_DOWNARROW))
	{
		_yellowfancon->setPosition(GVector2(32.0f, 356.0f));
	}
	else if (_input->isKeyDown(DIK_X))
	{
		if (_introtable->getPositionX() > 0)
		{
			return;
		}
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BRIDGE) == false)
		{
			SoundManager::getInstance()->Play(eSoundId::DESTROY_BRIDGE);
		}
		if (_yellowfancon->getPositionY() == 324)
		{
			_ok = 1;
		}
		else if (_yellowfancon->getPositionY() == 356)
		{
			_ok = 2;
		}
	}
}
bool IntroScene::init()
{
	_yellowfancon = SpriteManager::getInstance()->getSprite(eID::YELLOWFALCON);
	_yellowfancon->setPosition(GVector2(32.0f, 324.0f));
	_yellowfancon->setScale(SCALE_FACTOR);
	_yellowfancon->setOrigin(VECTOR2ZERO);

	_introtable = SpriteManager::getInstance()->getSprite(eID::MENU);
	_introtable->setPosition(GVector2(512.0f, 0));
	_introtable->setScale(SCALE_FACTOR);
	_introtable->setOrigin(GVector2(0.0f,1.0f));

	_playOption = new TextSprite(eID::FONTFULL, "START", GVector2(80.0f, 320.0f));
	_playOption->init();
	_playOption->setScale(SCALE_FACTOR);
	_playOption->setOrigin(VECTOR2ZERO);
	_playOption->setOpacity(0.8f);

	_chosenOption = new TextSprite(eID::FONTFULL, "SELECT STAGE", GVector2(80.0f, 352.0f));
	_chosenOption->init();
	_chosenOption->setScale(SCALE_FACTOR);
	_chosenOption->setOrigin(VECTOR2ZERO);
	_chosenOption->setOpacity(0.8f);

	_movement = new Movement(VECTOR2ZERO, GVector2(-200.0f, 0.0f), _introtable);

	_flash = new StopWatch();
	_access = new StopWatch();

	_ok = 0;
	_draw = true;
	_drawyellowFalcon = false;

	SoundManager::getInstance()->Play(eSoundId::INTRO);

	return true;
}

void IntroScene::onKeyPressed(KeyEventArg* key_event)
{

}
void IntroScene::update(float dt)
{
	_movement->update(dt);

	_playOption->setPosition(GVector2(_introtable->getPositionX() + 80.0f, _playOption->getPosition().y));
	_chosenOption->setPosition(GVector2(_introtable->getPositionX() + 80.0f, _chosenOption->getPosition().y));

	if (_introtable->getPositionX() < 0)
	{
		_movement->setVelocity(VECTOR2ZERO);
		_drawyellowFalcon = true;
	}

	if (_ok != 0)
	{
		if (_flash->isTimeLoop(120))
		{
			_draw = !_draw;
		}
	}

	if (_ok == 1)
	{
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BRIDGE) == false)
		{
			auto play = new BeginPlayScene(0, 3, 1);
			SceneManager::getInstance()->replaceScene(play);
		}
	}
	else if (_ok == 2)
	{
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BRIDGE) == false)
		{
			auto choice = new SelectionScene();
			SceneManager::getInstance()->replaceScene(choice);
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

	if (_ok == 0)
	{
		_playOption->draw(spriteHandle);
		_chosenOption->draw(spriteHandle);
	}
	else 
	{
		if (_ok == 1)
		{
			if (_draw == true)
			{
				_playOption->draw(spriteHandle);
			}
			_chosenOption->draw(spriteHandle);
		}
		else if (_ok == 2)
		{
			if (_draw == true)
			{
				_chosenOption->draw(spriteHandle);
			}
			_playOption->draw(spriteHandle);
		}
	}
}

void IntroScene::release()
{
	SAFE_DELETE(_playOption);
	SAFE_DELETE(_chosenOption);
	SAFE_DELETE(_yellowfancon);
	SAFE_DELETE(_introtable);
	SAFE_DELETE(_movement);
	SAFE_DELETE(_access);

	if (_input != nullptr)
		__unhook(_input);
}

IntroScene::IntroScene()
{

}

IntroScene::~IntroScene()
{
}