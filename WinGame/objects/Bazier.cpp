
#include "Bazier.h"


Bazier::Bazier()
{
}
GVector2 Bazier::getPositionInBazier(float ratio)
{
	int n = _anchorPoints.size() - 1;
	GVector2 result(0, 0);
	GVector2 temp(0, 0);
	float coefficient = 0.0f; // Giá trị hệ số tam giác pascal
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	for (int i = 0; i <= n ; i++)
	{
		temp = this->_anchorPoints[i];
		coefficient = CalculateCoefficient(n, i) * pow(1 - ratio, n - i) * pow(ratio, i);
		deltaX = coefficient * temp.x;
		deltaY = coefficient * temp.y;
		result.x += deltaX;
		result.y += deltaY;
	}

	return result;
}

void Bazier::insertAnchor(GVector2 point)
{
	_anchorPoints.push_back(point);
}

GVector2& Bazier::getPointAt(int index)
{
	if (index >= _anchorPoints.size())
	{
		throw;
	}
	return _anchorPoints.at(index);
}

void Bazier::updatePoint(int index, GVector2 value)
{
	if (index >= _anchorPoints.size())
	{
		throw;
	}
	_anchorPoints.at(index).x = value.x;
	_anchorPoints.at(index).y = value.y;
}

GVector2 Bazier::getPositionInBazier(float ratio, GVector2 anchorpoints[], int count)
{
	GVector2 result(0, 0);
	GVector2 temp(0, 0);
	float coefficient = 0.0f; // Giá trị hệ số tam giác pascal
	float deltaX = 0.0f;
	float deltaY = 0.0f;
	int n = count - 1;
	for (int i = 0; i <= n; i++)
	{
		temp = anchorpoints[i];
		coefficient = CalculateCoefficient(n, i) * pow(1 - ratio, n - i) * pow(ratio, i);
		deltaX = coefficient * temp.x;
		deltaY = coefficient * temp.y;
		result.x += deltaX;
		result.y += deltaY;
	}
	return result;
}

Bazier::~Bazier()
{
}

