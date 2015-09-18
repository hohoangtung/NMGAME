#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include "define.h"
#include "Sprite.h"
#include "Transformable.h"
#include "..\objects\IComponent.h"

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

	void addFrameRect(RECT rect);
	void addFrameRect(float left, float top, int width, int height);
	void addFrameRect(float left, float top, float right, float bottom);

private:
	int						_index;									// số thứ tự frame
	int						_totalFrames;

	float					_timeAnimate;							// thời gian chuyển giữa các frame
	float					_timer;

	bool					_canAnimate;

	Sprite*					_spriteSheet;							// ref to object's sprite
	vector<RECT>			_frameRectList;
	string					_nameAnimation;
	RECT					_currentRect;

	void updateTransform();
};

#endif // !__ANIMATION_H__
