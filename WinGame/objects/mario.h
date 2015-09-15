
#ifndef __BILL_H__
#define __BILL_H__
#include "BaseObject.h"
#include "PlayScene.h"
#include "IComponent.h"
#include "..\FrameWork\StopWatch.h"
#include <map>
using namespace std;

/*
	class Mario để ví dụ cách xây dựng một object
*/
class Mario : public BaseObject
{
public:
	// khi tạo object mới thì thêm vào eID trong file define.h một ID mới
	// mỗi đối tượng sẽ có ID riêng biệt
	Mario() : BaseObject(eID::MARIO){}
	~Mario(){  }
	
	void init()
	{
		_sprite = SpriteManager::getInstance()->getSprite(eID::MARIO);
		//this->_sprite->getAnimation()->createAnimate(0, 2, 0.08f);
		this->_sprite->setPosition(300, 500);
		GVector2 v(0, -10);
		GVector2 a(0, -70);
		this->_listComponent.push_back(new Movement(v, a, this->_sprite));
		this->_listComponent_.insert(pair<string, IComponent*>("Movement", new Movement(v, v, this->_sprite)));
	
		stopwatch = new StopWatch();
	}
	void update(float deltatime)
	{
		// some logic here
		for each (auto component in _listComponent)
		{
		//	component->update(deltatime);
		}
		for (auto it : _listComponent_)
		{
			// it kiểu pair <string, IComponent*>
			// it.second là kiểu IComponent*
			// it.first là kiểu string
			it.second->update(deltatime);
		}
		if(stopwatch->isTimeLoop(8000.0f))
		{
			Movement* move = (Movement*)this->getComponent("Movement");
			GVector2 v = move->getVelocity();
			move->setVelocity(GVector2(-v.x, -v.y));
		}
		//this->_sprite->setIndex(++i % 3); // test

		// or some other logic here
		//this->_sprite->update(deltatime);
	}
	void draw(LPD3DXSPRITE spritehandle)
	{
		// View port để chuyển hệ toạ độ như toạ độ đê-cac.
		this->_sprite->render(spritehandle, PlayScene::getViewport());
	}
	void release() {}

	IComponent* getComponent(string componentName)
	{
		return _listComponent_.find(componentName)->second;
	}
private:
	int i;
	vector<IComponent*> _listComponent;
	map <string, IComponent*> _listComponent_;// nên 
	StopWatch *stopwatch;
};

#endif // !__BILL_H__
