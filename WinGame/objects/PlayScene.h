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
#include "../Tiles/Map.h"

#include "BaseObject.h"
#include "Bill.h"
#include "Mario.h"
#include "RedCannon.h"
#include "WallTurret.h"
#include "Soldier.h"
#include "Falcon.h"
#include "AirCraft.h"
#include "Rifleman.h"
#include "Bridge.h"
#include "Grass.h"
#include "Land.h"
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
	//static Viewport* getViewport();

	// Trả về một đối tượng theo id.
	// id: kiểu enum eID, định danh một đối tượng.
	// return: đối tượng cần tìm.
	BaseObject* getObject(eID id);

	// Lấy đối tượng bill.
	Bill* getBill();
private:
	//static Viewport* _viewport;
	void destroyobject();				// kiển tra nếu object hết hạn sử dụng thì phá huỷ đối tượng
	Sprite* sprite;
	Text* _text;
	vector<BaseObject*> _listobject;
	vector<IControlable*> _listControlObject;
	Animation* _animation;
	Map* background;

	// Trỏ đến bill, một số đối tượng cần truyền bill vào để xử lý, lấy ở đây.
	BaseObject* _bill; 

};

#endif // !__PLAY_SCENE_H__
