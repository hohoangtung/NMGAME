

#ifndef __BULLET_MANAGER__
#define __BULLET_MANAGER__
#include "BaseObject.h"
#include "Bullet.h"

/*
*	Author: Tùng Hồ
*	Date Modified: 2/12/2015
*	Purpose:
*			Class để quản lý danh sách các đạn,
*			
*			Hàm update: gọi từng thành phần để update
*			Hàm draw: gọi từng thành phần để draw
*			Hàm checkcolision: gọi từng thành phần để check
*			Hàm release: giải phóng những đối tượng hết hiệu lực.
*
*	Cách sử dụng:
*		Được khởi tạo trong playscene, được gọi update, checkcollision, draw trong playscnee.
*		Khi một đối tượng bắn đạn ra chỉ cần gọi
*
*		BulletManager::insertBullet(new Bullet(...));
*		và không cần thiết thực hiện bất kì câu lệnh nào khác.
*		
*	Note:
*		Để xứ lý hành động đạn bắn trúng đích, chỉ xử lý trên lớp bullet không xử lý trên đối tượng bị bắn trúng
*/
class BulletManager : public BaseObject
{
public:
	// Khôgn cần init bullet
	static void insertBullet(Bullet* bullet);

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void release();
	RECT getBounding();
	float checkCollision(BaseObject * object, float dt);

	BulletManager();
	~BulletManager();

private:
	static list<Bullet*> _listBullet;

};

#endif // !__BULLET_MANAGER__
