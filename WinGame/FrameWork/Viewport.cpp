#include "Viewport.h"

Viewport::Viewport()
{
}

Viewport::Viewport(float x, float y)
{
	_positionWorld.x = x;
	_positionWorld.y = y;
}

Viewport::Viewport(float x, float y, float width, float height) : Viewport(x, y)
{
	_width = width;
	_height = height;
}

Viewport::~Viewport()
{
}

void Viewport::setPositionWorld(GVector2 position)
{
	if (_positionWorld != position)
		_positionWorld = position;
}

GVector2 Viewport::getPositionWorld()
{
	return _positionWorld;
}

float Viewport::getWidth()
{
	return _height;
}

float Viewport::getHeight()
{
	return _width;
}
