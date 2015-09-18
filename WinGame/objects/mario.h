
#ifndef __BILL_H__
#define __BILL_H__
#include "BaseObject.h"
#include "PlayScene.h"
#include "IComponent.h"
#include "..\FrameWork\StopWatch.h"
#include "..\FrameWork\Event.h"
#include "..\debug.h"
#include <functional>
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
	Mario() : BaseObject(eID::MARIO){

	}
	~Mario(){  }


	void init()
	{
		_sprite = SpriteManager::getInstance()->getSprite(eID::MARIO);
		this->_sprite->setPosition(400, 500);
		GVector2 a(0, 0);
		GVector2 v(0, 0);
		Movement* movement = new Movement(a, v, this->_sprite);
		this->_listComponent["Movement"] = movement;

		// thông thường các đối tượng chuyển động sẽ có trong lực. 
		this->_listComponent["Gravity"] = new Gravity(GVector2(0, -40), movement);
		
		// CÁCH SỬ DỤNG __event
		// đối với các kiểu nút nhấn thì ta dùng event để xử lý.
		// hàm thực hiện event phải có prototype như hàm Mario::A. nên để private
		// keyPressed là event được định nghĩa trong InputController
		// _input là một đối tuọng Input Controler được kế thừa từ IControlable
		__hook(&InputController::__eventkeyPressed, _input, &Mario::A);
		__hook(&InputController::__eventkeyReleased, _input, &Mario::B);

		// CÁCH SỬ DỤNG class Event
		_input->_keyPressed += (EventFunction)(&Mario::X);
		_input->_keyReleased += (EventFunction)(&Mario::Y);

		// lưu ý. sự kiện key release được kích hoạt khi ta vừa buôn tay khỏi phím. chứ không phải lúc key không được nhấn.

		// seemore:
		// https://msdn.microsoft.com/en-us/library/ee2k0a7d.aspx
		// https://msdn.microsoft.com/en-us/library/6f01ek09.aspx

		// mình làm biến làm animation nên làm setIndex cho sprite :p
		curIndex = 0;
	}
	void updateInput(float deltatime)
	{
		Movement* move = (Movement*)this->getComponent("Movement");
		// no any key down
		// do something

		// mình chỉ ví dụ cách dùng isKeyDown thôi. phần logic các bạn tự làm. 

		// key down là nút được nhấn và giữ. thường dùng cho di chuyển.
		// khác với khi nhảy. khi nhảy chỉ nhấn một lần thì không dùng keydown
		if (_input->isKeyDown(DIK_RIGHT))
		{
			OutputDebugString(L"RIGHT (key down)\n");
			move->setVelocity(GVector2(150, 0));
			this->setStatus(eStatus::RUNNING);
			this->_sprite->setScale(GVector2(1, 1));
			return;	// không return
		}
		if (_input->isKeyDown(DIK_LEFT))
		{
			OutputDebugString(L"LEFT (key down)\n");
			move->setVelocity(GVector2(-150, 0));
			this->setStatus(eStatus::RUNNING);
			this->_sprite->setScale(GVector2(-1, 1));
			return;	// không return
		}
		move->setVelocity(GVector2(0, move->getVelocity().y));
		if (this->getStatus() != eStatus::JUMPING)
			this->setStatus(eStatus::NORMAL);
	}
	void update(float deltatime)
	{
		for (auto it : _listComponent)
		{
			// it kiểu pair <string, IComponent*>
			// it.second là kiểu IComponent*
			// it.first là kiểu string
			it.second->update(deltatime);
		}

		this->_sprite->setIndex(_runanimation[(curIndex) % sizeof(_runanimation)/ sizeof(int)]); // test
		// các bạn hiểu tư tưởng là dùng status để định nghĩa logic cho đối tượng là được.
		// đừng quan tâm logic của con mario này. tại hơi sai :))
		switch (this->getStatus())
		{
		case NORMAL:
		{
			curIndex = 0; 
			Gravity* gravity = (Gravity*) this->getComponent("Gravity");
			gravity->setStatus(eGravityStatus::FALLING__DOWN);
			break;
		}
		case RUNNING:
			curIndex++; break;
		case JUMPING:
		{
 			Movement* move = (Movement*)this->getComponent("Movement");
			Gravity* gravity = (Gravity*) this->getComponent("Gravity");
			gravity->setStatus(eGravityStatus::SHALLOWED);
			auto veloc = move->getVelocity();
			move->setVelocity(veloc + GVector2(0, 800));
			this->setStatus(NORMAL);

			break;
		}
		default:
			break;
		}
	}
	void draw(LPD3DXSPRITE spritehandle)
	{
		// View port để chuyển hệ toạ độ như toạ độ đê-cac.
		this->_sprite->render(spritehandle, PlayScene::getViewport());

	}
	void release()
	{
		for (auto it : _listComponent)
		{
			delete it.second;
		}
		_listComponent.clear();
	}

	IComponent* getComponent(string componentName)
	{
		return _listComponent.find(componentName)->second;
	}
private:
	void A(KeyEventArg* e)
	{
		if (e == NULL)
			return;
		if (e->_key == DIK_SPACE)
		{
			OutputDebugString(L"A (dùng __event)\n");
			this->setStatus(eStatus::JUMPING);
		}
	}
	void B(KeyEventArg* e)
	{
		if (e->_key == DIK_SPACE)
		{
			OutputDebugString(L"B (dùng __event)\n");
		}
	}
	static void X(KeyEventArg* e)
	{
		// dùng class Event thì hàm buộc phải là static
		// vì static không được truy xuất thành phần non-static nên không thích hợp sử dụng trong đối tượng
		if (e->_key == DIK_SPACE)
		{
			OutputDebugString(L"X (dùng class Event)\n");
		}
	}
	static void Y(KeyEventArg* e)
	{
		if (e->_key == DIK_SPACE)
		{
			OutputDebugString(L"Y (dùng class Event)\n");
		}
	}
	map <string, IComponent*> _listComponent;
	int curIndex;
	static const int _runanimation[3];
};
const int Mario::_runanimation[3] = { 0, 1, 2 };
#endif // !__BILL_H__

// Have fun!!!
