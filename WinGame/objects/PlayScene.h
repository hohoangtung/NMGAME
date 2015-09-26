#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include <vector>
#include "Scene.h"
#include "../FrameWork/Sprite.h"
#include "../FrameWork/SpriteManager.h"
#include "../FrameWork/Managers/SceneManager.h"
#include "../FrameWork/Text.h"
#include "../FrameWork/Animation.h"
#include "../debug.h"
#include "../Map/Map.h"

#include "BaseObject.h"
#include "Bill.h"
#include "Mario.h"
#include "RedCannon.h"
#include "Soldier.h"
#include "Falcon.h"
#include "AirCraft.h"
using namespace std;
US_FRAMEWORK

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

	void setViewport(Viewport* viewport);
	static Viewport* getViewport();
private:
	static Viewport* _viewport;
	Sprite* sprite;
	Text* _text;
	vector<BaseObject*> _listobject;
	vector<IControlable*> _listControlObject;
	Animation* _animation;
	Map* background;
};

#endif // !__PLAY_SCENE_H__
