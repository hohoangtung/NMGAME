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

/*
	Kiểm tra hai hình chữ nhật có chồng lên nhau không.
	Dùng trong hệ top left
*/
bool isIntersectd(RECT rect1, RECT rect2);
/*
* Kiểm tra rect1 có chứa rect2 không.
* @return: true nếu rect1 chứa rect2
*/
bool isContains(RECT rect1, RECT rect2);

/*
* Tính khoảng cách giữa hai điểm
* @return: độ dài khoảng cách được tính theo công thức: d = √((x1 - x2 )² + (y1 - y 2)²)
*/
float getdistance(D3DXVECTOR2 p1, D3DXVECTOR2 p2);

std::vector<std::string> splitString(const std::string & input, char seperate);

/*
	Kiểm tra điểm có nằm trong hcn hay ko
	@rect: hcn tính theo bottom-left
	@point: điểm được kiểm tra
*/
bool isContain(RECT rect, D3DXVECTOR2 point);

double CalculateCoefficient(int n, int k);


#endif // !__UTILS_H__
