#ifndef __CONTRA_GAME_H__
#define __CONTRA_GAME_H__

#include "Game.h"
#include "FrameWork/Managers/SoundManager.h"
#include "FrameWork/Managers/SceneManager.h"
#include "objects/Scenes/PlayScene.h"
#include "objects/Scenes/IntroScene.h"

class ContraGame : public Game
{
public:
	ContraGame(HINSTANCE hInstance, LPWSTR title);
	~ContraGame();

	void init();						// init your objects
	void release();						// release your objects

	void updateInput(float deltatime);	// update input, include played objects and menu button
	void update(float deltatime);		// update your objects
	void draw();						// draw your objects
	void loadResource();

private:

};

#endif // !__CONTRA_GAME_H__
