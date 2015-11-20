

// Nơi chứa các hàm hướng thủ tục.


#ifndef __UTILS_H__
#define __UTILS_H__

#include <d3dx9.h>

/*
	Kiểm tra hai hình chữ nhật có chồng lên nhau không.
	rect1, rect2: hai hình chữ nhật cần kiểm tra.
	return: true nếu có chồng lên nhau, ngược lại là false
*/
bool isRectangleIntersected(RECT rect1, RECT rect2);

#endif // !__UTILS_H__
