

#ifndef __GAMEOVER_SCENE__
#define __GAMEOVER_SCENE__

#include "..\FrameWork\Sprite.h"
#include "..\FrameWork\SpriteManager.h"
#include "..\FrameWork\Managers\SoundManager.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\StopWatch.h"

#include "IComponent.h"
#include "BaseObject.h"
#include "TextSprite.h"
#include "Scene.h"

[event_receiver(native)]
class GameOverScene : public Scene, public IControlable
{
public:
	GameOverScene(int score, int prePlayScene);
	~GameOverScene();

	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime) override;
	void onKeyPressed(KeyEventArg* key_event);

private:
	Sprite* _blank;
	Sprite* _yellowfancon;
	Sprite* _background;

	StopWatch* _flash;
	TextSprite* _textscore;
	TextSprite* _texthighscore;
	int _highscore;

	int _score;
	int _index;
	int _prePlayScene;
};



#endif // !__GAMEOVER_SCENE__
