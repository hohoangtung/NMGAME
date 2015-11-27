

#include "utils.h"

bool isRectangleIntersected(RECT rect1, RECT rect2)
{
	float left = rect1.left - rect2.right;
	float top = rect1.top - rect2.bottom;
	float right = rect1.right - rect2.left;
	float bottom = rect1.bottom - rect2.top;

	//  Có chồng lên nhau khi
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return false;
}

// Cắt chuỗi.
// inout: chuỗi cần cắt.
// seperate: kí tự dùng để cắt chuỗi
// return: mãng các chuỗi đã được cắt.
std::vector<string> splitString(const string & input, char seperate)
{
	vector<string> output;
	std::stringstream ss(input);

	string item;

	while (std::getline(ss, item, seperate))
	{
		output.push_back(item);
	}

	return output;
}

bool isContains(RECT rect1, RECT rect2)
{
	float left = rect1.left - rect2.left;
	float top = rect1.top - rect2.top;
	float right = rect1.right - rect2.right;
	float bottom = rect1.bottom - rect2.bottom;

	if (left * right > 0)
	{
		return false;
	}
	if (top * bottom > 0)
	{
		return false;
	}
	return true;
}