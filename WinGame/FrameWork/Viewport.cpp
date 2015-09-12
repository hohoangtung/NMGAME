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

GVector3 Viewport::getPositionInViewport(GVector3* position)
{
	D3DXMATRIX mt;
	D3DXVECTOR4 posViewport;

	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = (-1) * _positionWorld.x;
	mt._42 = _positionWorld.y;
	
	D3DXVec3Transform(&posViewport, position, &mt);

	return GVector3(posViewport.x, posViewport.y, posViewport.z);
}
