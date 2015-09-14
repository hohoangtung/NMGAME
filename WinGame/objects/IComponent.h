
#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__
#include "..\FrameWork\define.h"
#include "..\FrameWork\Sprite.h"
US_FRAMEWORK // = using namespace framework


/*
	base component
	các hiệu ứng nào có ảnh hưởng đến object suốt game thì kế thừa lại.
	hãy chắc rằng bạn kiểm soát được hoàn toàn khả năng update của class kế thừa
	(nên test bằng cách update rời trước)
	Component được khai báo trong đối tượng chứ không được khai báo trong BaseObject.

	bên trong object nếu muốn sử dụng Component thi có thê sử dụng vector, map hoặc pair, tuỳ thích.
	Nhưng hãy chắc rằng bận hiểu cách sử dụng container.
	seemore about container:

	http://www.cplusplus.com/reference/map/map

	Xem ví dụ class Mario, file mario.h
*/
class IComponent
{
public:
	virtual void update(float deltatime) = 0;
};

/*
	Hiệu ứng chuyển động theo gia tốc và vận tốc.
	Khi không chuyển động thì gia tốc và vận tốc bằng 0
	Khi sử dụng:
		Khởi tạo bên trong init của object.
		Truyền refSprite = Sprite của object đó.
*/
class Movement : public IComponent
{
public:
	Movement(GVector2 accel, GVector2 veloc, Sprite* refSprite);
	void update(float deltatime);

	void setAccelerate(GVector2 accel);
	void setVelocity(GVector2 veloc);
	GVector2 getAccelerate();
	GVector2 getVelocity();
private:
	GVector2 _accelerate;
	GVector2 _velocity;
	Sprite* _refSprite;
};
#endif // !__ICOMPONENT_H__
