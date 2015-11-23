
/*
author: Ho Hoang Tung
updated: 19/11/2015

Lớp Bridge dùng để tạo một đối tượng cây cầu bị nổ ở màn 1 trong game Contra.
Trong lớp này có:
	
	Các phương thức kế thừa từ lớp BaseObject.
	Implement phương thức IControlable, dùng để test. Nếu release sẽ xoá.

	Inner class QuadExplose: dùng để tạo một vụ nổ có 4 đám cháy. Mỗi đám cháy là một thể hiện của lớp Explose

	Các thuộc tính private:
	(Note dài, nên xem ở chế độ textwrap)

		Tên					Kiểu						Miêu tả

		_matrixIndex		int**						Dùng để lưu các giá trị index để tạo thành một hình ảnh cây cầu, mỗi giá trị được map đến một ảnh nhỏ được chứa trong sprite của baseobject, mỗi giá trị trong mảng sẽ được vẽ như một 'pixel' trên màn hình.

		_stopwatch			StopWatch					Dùng để đếm thời gian để tạo ra các vụ nổ liên tiếp

		_transform			Transformable				Dùng để custom quá trình render, bình thường thì chỉ cần vẽ sprite trong baseobject, ở đây ta vẽ nhiều phần tử để tạo ra một hình ảnh lớn nên dùng transform là một đối tượng chứa các dữ liệu transform (position, scale, rotate, ...) chung cho cả cây cầu.

		_listComponent		map<string, IComponent*>	Dùng để tạo thành phần vật lý va chạm trên cây cầu.

		_wave				int							Dùng để phục vụ cho thuật toán nổ cầu.

*/


#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include "BaseObject.h"
#include "Explosion.h"

// nếu sửa max_wave thì phải sửa _matrixIndex
#define MAX_WAVE 4		// số lần nổ
#define DELAYTIME 500.0f

#define BRIDGE_POSITION GVector2(200.0f, 300.0f)	// test value
[event_receiver(native)] 
class Bridge : public BaseObject
{
public:
	Bridge(GVector2 position);
	~Bridge();

	// Các phương thức kế thừa từ base-class.
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();

	// Theo dỗi bill. Nếu hắn đến gần cầu thì cho nổ cầu.
	void trackBill(Bill* bill);

	// Xử lý nổ cầu.
	// deltatime: thời gian update vòng lặp game (milisecond)
	void burst(float deltatime);

	void setPosition(GVector2 position) override;
	GVector2 getPosition() override;
	RECT getBounding() override;

private:
	static int _matrixIndex[2][MAX_WAVE * 2];
	StopWatch* _stopwatch;
	Transformable *_transform;
	map<string, IComponent*> _listComponent;
	int _wave;
private:
	class QuadExplose : public BaseObject
	{
	public:
		QuadExplose(GVector2);
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE, Viewport*);
		void release();
		void setPosition(GVector2 position);
		GVector2 getPosition();
		void reset();
	private:
		Transformable* _transform;
		BaseObject* _explosion1;
		BaseObject* _explosion2;
		BaseObject* _explosion3;
		BaseObject* _explosion4;
		float _timer;

	};
	QuadExplose* _explode;
};
#endif // !__BRIDGE_H__
