
#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include <vector>
#include "Scene.h"
#include "../FrameWork/Sprite.h"
#include "../FrameWork/SpriteManager.h"
#include "../FrameWork/Managers/SceneManager.h"
#include "../FrameWork/Managers/SoundManager.h"
#include "../FrameWork/Text.h"
#include "../FrameWork/Animation.h"
#include "../debug.h"
#include "../Tiles/Map.h"
#include "../QNode.h"

#include "BaseObject.h"
#include "Bill.h"
#include "RedCannon.h"
#include "Soldier.h"
#include "Falcon.h"
#include "AirCraft.h"
#include "Rifleman.h"
#include "Bridge.h"
#include "Land.h"
#include "WallTurret.h"
#include "BulletManager.h"
#include "Boss.h"
#include "RockFly.h"
#include "ScubaSoldier.h"
#include "WallTurret_inactived.h"
#include "RedCannon_appear.h"
#include "Fire.h"
#include "RockFall.h"

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

	// Danh sách đối tượng dùng để tạo quadtree.
	map <string, BaseObject*> _mapobject;

	// Danh sách các đối tượng hoạt động rộng không thể đưa vào quadtree.
	// (Ví dụ main character)
	vector<BaseObject*> _listobject;

	// Danh sách đối tượng nằm trong vùng active.
	// Mỗi vòng lặp sẽ được kiểm tra chỉnh sửa tại update, và sử dụng lại tại draw.
	vector<BaseObject*>   _active_object;

	vector<IControlable*> _listControlObject;
	Animation* _animation;
	Map* background;

	// quadtree
	QNode* _root;
	// Trỏ đến bill, một số đối tượng cần truyền bill vào để xử lý, lấy ở đây.
	BaseObject* _bill; 

	void updateViewport(BaseObject* objTracker);
};

#endif // !__PLAY_SCENE_H__
