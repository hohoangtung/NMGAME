

#ifndef __INTRO_SCENE__
#define __INTRO_SCENE__

#include "..\FrameWork\Sprite.h"
#include "..\FrameWork\SpriteManager.h"
#include "..\FrameWork\Managers\SoundManager.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\StopWatch.h"

#include "IComponent.h"
#include "BaseObject.h"
#include "Scene.h"

[event_receiver(native)]
class IntroScene : public Scene, public IControlable
{
public:
	IntroScene();
	~IntroScene();


	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime) override;

private:
	Sprite* _blank;
	Sprite* _yellowfancon;
	Sprite* _introtable;
	Movement* _movement;

	void onKeyPressed(KeyEventArg* key_event);
	StopWatch* _flash;
	StopWatch* _access;
	int _ok;
	int _draw;
	bool _drawyellowFalcon;
};
#endif // !__INTRO_SCENE__
