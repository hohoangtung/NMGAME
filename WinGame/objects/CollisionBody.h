#ifndef __COLLISION_BODY__
#define __COLLISION_BODY__

#include "BaseObject.h"
#include "IComponent.h"
#include <queue>

/*
by Luu The Vinh
updated: 2/10/2015

Sử dụng cho đối tượng muốn kt va chạm
- Thêm nó như là một Component vào object
- Có 2 Event là:
	@onCollisionBegin: sẽ được gọi khi bắt đầu va chạm
	@onCollisionEnd: sẽ được gọi khi kết thúc va chạm
- Đối tượng sử dụng Collision body muốn dùng event thì gán 2 event trên để sử dụng
	- Thêm [event_receiver(native)] vào trước class định nghĩa object
	- Viết hàm có THAM SỐ như sau:
		+ void onCollisionBegin(CollisionEventArg* collision_event);
		+ void onCollisionEnd(CollisionEventArg* collision_event);
	- Gán hàm cho Collision Body
		+ __hook(&CollisionBody::onCollisionBegin, <collision body của object> , & <tên lớp>::<tên hàm>);
	- Việc kiểm tra và sử lý va chạm cho đối tượng đó sẽ làm bên trong 2 hàm BEGIN và END.
	- CollisionEventArg: 
		+ Đối tượng va chạm với đối tượng hiện tại.
		+ _sideCollision: phía va chạm của đối tượng kia.

Tham khảo class Bill để xem chi tiết.
Còn một số lỗi đang làm tiếp. :D
*/

class CollisionEventArg : public EventArg
{
public:
	CollisionEventArg(BaseObject* object)
	{
		_otherObject = object;
		_sideCollision = eDirection::NONE;
	}

	BaseObject* _otherObject;
	eDirection _sideCollision;
private:

};

[event_source(native)]
class CollisionBody : public IComponent
{
public:
	CollisionBody(BaseObject* target);
	~CollisionBody();

	/*
	kiểm tra va chạm với object khác, gọi event Begin, End.
		@otherObject: object cần kt va chạm
		@dt: delta time của mỗi frame
	*/
	void checkCollision(BaseObject* otherObject, float dt);

	/*
	kiểm tra va chạm với object khác lấy được hướng va chạm, KO  gọi event Begin, End.
		@otherObject: object cần kt va chạm
		@direction: lấy hướng va chạm của otherObject
		@dt: delta time của mỗi frame
	*/
	bool checkCollision(BaseObject* otherObject, eDirection& direction, float dt);

	bool isColliding(BaseObject* otherObject);
	
	void update(float dt);

	__event void onCollisionBegin(CollisionEventArg* e);
	__event void onCollisionEnd(CollisionEventArg* e);

private:
	BaseObject* _target;

	float _dxEntry, _dyEntry, _dxExit, _dyExit;
	float _txEntry, _tyEntry, _txExit, _tyExit;

	map<BaseObject*, bool> _listColliding;

	float isCollide(BaseObject* otherObject, eDirection& direction, float dt);
	bool isColliding(RECT myRect, RECT otherRect);
	bool isColliding(BaseObject* otherObject, float& moveX, float& moveY, float dt);

	/*
	Cập nhật target position khi va chạm
		@otherObject: đối tượng va chạm
		@direction: hướng bị va chạm của otherObject
		@withVelocity: TRUE khi kt va chạm với vận tốc, tham số move ko cần. FALSE khi va chạm bằng kt RECT
		@move: khoảng chồng lấp của 2 object.
	*/
	void updateTargetPosition(BaseObject* otherObject, eDirection direction, bool withVelocity, GVector2 move = GVector2(0, 0));

	RECT getSweptBroadphaseRect(BaseObject* object, float dt);
	eDirection getSide(BaseObject* otherObject);
};

#endif // !__COLLISION_BODY__
