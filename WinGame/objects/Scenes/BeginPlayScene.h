

#ifndef __BEGIN_STAGE3__
#define __BEGIN_STAGE3__

#include "..\..\FrameWork\Sprite.h"
#include "..\..\FrameWork\SpriteManager.h"
#include "..\..\FrameWork\Managers\SoundManager.h"
#include "..\..\FrameWork\InputController.h"
#include "..\..\FrameWork\StopWatch.h"
#include "..\TextSprite.h"
#include "Stage3.h"

class HighScore
{
public:
	static char* filehighscore;
	static int loadHighScore(const char* fileInfoPath);
	static bool saveHighScore(const char* fileInfoPath, int score);
};

class BeginPlayScene : public Scene
{
public:
	/*
	* Khởi tạo màn hình chờ stage 3
	* @score: điểm hiện tại.
	* @rest: số mạng còn lại.
	*/
	BeginPlayScene(int score, int rest, int stage = 1);
	~BeginPlayScene();

	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime);

private:
	
	static const float delaytime;
	int _score;
	int _highscore;
	int _rest;
	int _stage;

	Sprite* _waitscreen;
	StopWatch* _access;

	TextSprite* _textscore;
	TextSprite* _textrest;
	TextSprite* _texthighscore;
	TextSprite* _textStage;
	TextSprite* _textStageName;
};


#endif // !__BEGIN_STAGE3__
