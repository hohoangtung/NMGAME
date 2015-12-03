#ifndef __ANIMATION_H__
#define __ANIMATION_H__

/*
update 23/11/2015
Luu The Vinh

- Thêm tùy chỉnh lặp cho Animation
*/

#include <vector>
#include <stdarg.h>
#include "define.h"
#include "Sprite.h"
#include "..\objects\IComponent.h"
#include "SpriteManager.h"

US_FRAMEWORK

class Animation : public IComponent
{
public:
	~Animation();

	/*
	Tạo animation với sprite sheet
		@spriteSheet: spritesheet của chuyển động
		@timeAnimate: thời gian chuyển từng frame
	Sau khi tạo bằng contructor này phải addFrameRect nó mới có frame để chuyển động.
	*/
	Animation(Sprite* spriteSheet, float timeAnimate = 0.0f);

	/*
	Tạo aniation với spritesheet có frame đều nhau
		@spriteSheet: spritesheet của chuyển động
		@totalFrames: tổng số frame
		@cols: số cột
		@timeAnimate: thời gian chuyển từng frame
	*/
	Animation(Sprite* spriteSheet, int totalFrames, int cols, float timeAnimate = 0.0f);

	/*
	Chuyển qua frame kế tiếp
	*/
	void nextFrame();

	/*
	Truyền thứ tự frame cụ thể
	*/
	void setIndex(int index);
	
	/*
	Lấy giá trị index hiện tại
	*/
	int getIndex();

	/*
	Update animation
	*/
	void update(float dt);

	/*
	Vẽ chuyển động
	*/
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);

	/*
	Đặt thời gian chuyển frame
		@time: thời gian tính theo giây
	*/
	void setTimeAnimate(float time);
	
	/*
	Lấy giá trị thời gian chuyển frame
	*/
	float getTimeAnimate();

	/*
	Bắt đầu chuyển frame
	*/
	void start();

	/*
	Dừng chuyển frame
	*/
	void stop();

	void canAnimate(bool can);
	bool isAnimate();
	void addFrameRect(RECT rect);
	void addFrameRect(float left, float top, int width, int height);
	void addFrameRect(float left, float top, float right, float bottom);

	/*
	Tạo nhiều framerect liên tiếp nhau, không giới hạn tham số
		@id: ID của sprite
		@firstRectName,...: các tên của frame rect
	Sau khi truyền xong các tên, kết thúc PHẢI bằng NULL để xác định được kết thúc.
	*/
	void addFrameRect(eID id, char* firstRectName, ...);

	void setPosition(GVector2 p)
	{ 
		// không làm gì TT.TT
		// để tạm ở đây cho hết lỗi, nếu setPosition thì set ở sprite reference tới
	}

	void setLoop(bool isLoop);
	bool isLoop();
	void restart();

	/*
	Tạo hiệu ứng chớp cho sprite, sau khi set enable = true, phải đặt giá trị thay đổi ở hàm setValueFlashes
	*/
	void enableFlashes(bool enable);

	/*
	Gán giá trị chớp tắt
		@value: giá trị thay đổi để chớp, từ 0 đến < 1 (giá trị bình thường là 1)
	*/
	void setValueFlashes(float value);

private:
	int						_index;									// số thứ tự frame
	int						_totalFrames;

	float					_timeAnimate;							// thời gian chuyển giữa các frame
	float					_timer;

	bool					_canAnimate;
	bool					_isLoop;

	Sprite*					_spriteSheet;							// ref to object's sprite
	vector<RECT>			_frameRectList;
	string					_nameAnimation;
	RECT					_currentRect;

	bool					_canFlashes;
	float					_valueFlashes;
};

#endif // !__ANIMATION_H__
