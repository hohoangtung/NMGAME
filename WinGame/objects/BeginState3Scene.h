

#ifndef __BEGIN_STAGE3__
#define __BEGIN_STAGE3__

#include "..\FrameWork\Sprite.h"
#include "..\FrameWork\SpriteManager.h"
#include "..\FrameWork\Managers\SoundManager.h"
#include "..\FrameWork\InputController.h"
#include "..\FrameWork\StopWatch.h"

class BeginStage3Scene : public Scene
{
public:
	/*
	* Khởi tạo màn hình chờ stage 3
	* @score: điểm hiện tại.
	* @rest: số mạng còn lại.
	*/
	BeginStage3Scene(int score, int rest);
	~BeginStage3Scene();

	bool init() override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;
	void updateInput(float deltatime);

	int loadHighScore(const char* fileInfoPath);
	bool saveHighScore(const char* fileInfoPath, int score);
private:
	const char* filehighscore = "Resources\\highscore.txt";
	const float delaytime = 3000;
	int _score;
	int _highscore;
	int _rest;
	Sprite* _waitscreen;
	StopWatch* _access;
};


#endif // !__BEGIN_STAGE3__
