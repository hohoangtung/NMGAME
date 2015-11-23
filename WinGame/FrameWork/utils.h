﻿

// Nơi chứa các hàm hướng thủ tục.


#ifndef __UTILS_H__
#define __UTILS_H__

#include <d3dx9.h>
#include <string>
#include <vector>
#include <sstream>
using namespace std;
/*
	Kiểm tra hai hình chữ nhật có chồng lên nhau không.
	rect1, rect2: hai hình chữ nhật cần kiểm tra.
	return: true nếu có chồng lên nhau, ngược lại là false
*/
bool isRectangleIntersected(RECT rect1, RECT rect2);
std::vector<std::string> splitString(const std::string & input, char seperate);
#endif // !__UTILS_H__