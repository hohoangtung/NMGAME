
#ifndef __MARIO_H__
#define __MARIO_H__

#include "..\FrameWork\StopWatch.h"
#include "..\FrameWork\Event.h"
#include "..\debug.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "PlayScene.h"
#include <map>

using namespace std;

/*
	class Mario để ví dụ cách xây dựng một object
*/	
// nếu là đối tượng nhận điều khiển thì kế thừa IControlable. và trong playscene::init gọi 	_listControlObject.push_back(mario);
[event_receiver(native)] // for use __hook
class Mario : public BaseObject, public IControlable
{
public:
	// khi tạo object mới thì thêm vào eID trong file define.h một ID mới
	// mỗi đối tượng sẽ có ID riêng biệt
	Mario();
	~Mario();


	void init();
	void updateInput(float deltatime);
	void update(float deltatime);
	void draw(LPD3DXSPRITE spritehandle, Viewport*);
	void release();
	IComponent* getComponent(string componentName);
private:
	void A(KeyEventArg* e);
	void B(KeyEventArg* e);
	static void X(KeyEventArg* e);
	static void Y(KeyEventArg* e);

	map <string, IComponent*> _listComponent;
	int curIndex;
	static const int _runanimation[3];
};
#endif // !__MARIO_H__

// Have fun!!!
