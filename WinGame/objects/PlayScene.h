#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "Scene.h"
#include "../FrameWork/Sprite.h"
#include "../FrameWork/SpriteManager.h"
#include "../FrameWork/Managers/SceneManager.h"

US_FRAMEWORK;

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	bool init() override;
	void updateInput(float dt) override;
	void update(float dt) override;
	void draw(LPD3DXSPRITE spriteHandle) override;
	void release() override;

private:
	Sprite* sprite;
};

#endif // !__PLAY_SCENE_H__
