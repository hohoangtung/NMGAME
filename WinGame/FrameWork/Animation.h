#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "define.h"

class Animation
{
public:
	~Animation();

	/*
	Tạo animation cho sprite, dùng getFrameRect() cho Sprite vẽ frame hiện tại
		@totals: tổng số frame
		@cols: số cột
		@frameW: chiều ngang của mỗi frame hình
		@frameH: chiều dọc của mỗi frame hình
	*/
	Animation(int totals, int cols, int frameW, int frameH);

	/*
	Tạo animation cho srpite với frame bắt đầu và kết thúc cụ thể, chuyển động sẽ chạy từ startFrame->endFrame
		@startFrame: thứ tự frame bắt đầu (tính từ 0)
		@endFrame: thứ tự frame kết thúc
		@timeAnimate: thời gian chuyển mỗi frame hình
	*/
	void createAnimate(int startFrame, int endFrame, float timeAnimate);

	/*
	Chuyển qua frame kế tiếp
	*/
	void nextFrame();

	/*
	Lấy số thứ tự frame hiện tại
	*/
	int getCurrentFrame();

	/*
	Truyền thứ tự frame cụ thể
	*/
	void setIndex(int index);
	
	/*
	Lấy thông tin RECT frame hiện tại
	*/
	RECT getFrameRect();

	/*
	Update animation
	*/
	void update(float dt);

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
	Lấy chiều ngang của frame
	*/
	int getFrameWidth();

	/*
	Lấy chiều dọc của frame
	*/
	int getFrameHeight();

	/*
	Bắt đầu chuyển frame
	*/
	void start();

	/*
	Dừng chuyển frame
	*/
	void stop();

	void canAnimate(bool can);

private:
	int _totalFrames;					// tổng số frame
	int _startFrame;					// frame bắt đầu
	int _endFrame;						// frame kết thúc
	int _columns;						// số cột
	int _index;
	GVector2 _currentFrame;				// frame hiện tại

	int _frameWidth;
	int _frameHeight;

	float _timeAnimate;					// thời gian chuyển giữa các frame
	float _timer;

	RECT	_frameRect;

	bool _canAnimate;

	void setFrameRect();
	void setCurrentFrame();
};

#endif // !__ANIMATION_H__
