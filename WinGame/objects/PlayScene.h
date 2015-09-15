#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include <vector>
#include "Scene.h"
#include "../FrameWork/Sprite.h"
#include "../FrameWork/SpriteManager.h"
#include "../FrameWork/Managers/SceneManager.h"
#include "BaseObject.h"
#include "../FrameWork/Text.h"
#include "../FrameWork/Animation.h"

using namespace std;
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
	Text* _text;
	vector<BaseObject*> _listobject;
	Animation* _animation;
};

#endif // !__PLAY_SCENE_H__
