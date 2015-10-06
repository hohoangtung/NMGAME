/*
06.10.2015 - 7ung:
	Map là class quản lý việc vẽ map bằng tiles.
	
	một tileset được lưu như resource hình ảnh bao gồm nhiều ô nhỏ chứa một phần nhỏ của map. 
	vị trí các tile được lưu như một ma trận 2 chiều trên một file text. với giá trị các phần tử trên ma trận là (index của tile + 1). 0 = no tile here

	file text được load bằng phương thức init, truyền vào đường dẫn tương đối của file
	hình ảnh được load như sprite thông qua sprte manager.

	MOST BASIC:
	khi vẽ map. ta duyệt từ đầu đến cuối _mapIndex, kiểm tra index hiện tại là gì, sau đó set index cho sprite
		_sprite->setIndex(_mapIndex[i] - 1)
	khi đã biết i, từ i ta xác định vị trí cần vẽ map. (Mặc định map bắt đầu ở vị trí 0,0 và phát triển theo hệ toạ độ đề các)
			pos.x =	i % * _framewidth;
			pos.y = (_height - i / _width) * _frameheight;						// nếu có viewport thì dùng height - i / widht để lật toạ độ
			//pos.y = (i / _width) * (_frameheight - 1);				// không có viewport
			_sprite->setPosition(pos);

	UPGRADE LEVEL 1:
	duyệt mảng từ 0 đến _widht * _height có thể phát sinh đển hàng ngàn lần vẽ 
	ta giới hạn lại số lần vẽ bằng cách check nếu position vẽ nằm ngoài màn hình thì không vẽ
		PSEDUE CODE:
		for int i : _widht * _height
			if (bound.right < screenRect.left)
				continue;
			if (bound.bottom > screenRect.top)
				continue;
			if (bound.left > screenRect.right)
				continue;
			if (bound.top < screenRect.bottom)
				continue;
			draw here

	UPGRADE LEVEL 2:
	sau khi kiểm tra toạ độ. ta thấy rằng vẫn mất hàng ngàn lần lặp vòng for. 
	giải quyết bằng cách, kiểm tra toạ độ viewport trước khi lặp vòng for để xác định cặp giá trị i, j cho vòng for 2 lớp.
		int iBegin = max(screenRectEx.left / widthBound, 0);
		int iEnd = min(screenRectEx.right / widthBound + 1, _width);
		int jBegin = _height - min(screenRectEx.top / heightBound, _height);
		int jEnd = _height - max(screenRectEx.bottom / heightBound + 1, 0);

		// right và bottom cộng 1 vì cần có một tile vẽ tràn một phần ra khỏi màn hình.

	HAVE FUN
*/
#ifndef __MAP_H__
#define __MAP_H__6
#include <string>
#include <vector>
#include "..\FrameWork\define.h"
#include "..\FrameWork\SpriteManager.h"
using namespace std;

US_FRAMEWORK

class Map
{
public:
	void init(string filepath);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	Map();
	~Map();

private:
	/*
	 lưu ma trận 2 chiều (m x n ) bằng một mảng một chiều với kích thước bằng giá trị mxn. để tiết kiệm lưu trữ
	 truy xuất phần tử ở vị trí i,j  trên ma trận bằng cách truy xuất phần tử _mapIndex( t % _width, t / _width) với _width là số cột của ma trận.
	 khi đó i = t % _width và j = t / _width
	*/
	int* _mapIndex;		
	int _width;		// số tile chiều ngang
	int _height;	// số tile chiều dọc
	Sprite* _sprite;
	int _framewidth;	// độ rộng một frame (độ rộng của tile)
	int _frameheight;	// độ cao một frame (độ cao của tile)
	
};


#endif // !__MAP_H__
