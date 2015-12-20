
#include "Land.h"


Land::Land(int x, int y, int width, int height, eDirection physicBodyDirection, eLandType type) : BaseObject(eID::LAND)
{
	// X là left. Y là top
	this->_bound.top = y;
	this->_bound.left = x;
	this->_bound.bottom =  y - height;
	this->_bound.right = x + width;
	this->_type = type;
	BaseObject::setPhysicsBodySide(physicBodyDirection);

	if (type == eLandType::GRASS)
		_canJump = true;
	else if (type == eLandType::WATER)
		_canJump = false;

	_surface = nullptr;

	//create surface
	DeviceManager::getInstance()->getDevice()->CreateOffscreenPlainSurface(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&_surface,
		NULL
		);
}

void Land::init()
{
	this->_sprite = nullptr;
	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	//this->setPhysicsBodySide(eDirection::TOP);
}

void Land::update(float deltatime)
{
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
}

void Land::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	// do nothing.
	return;

	RECT r;
	auto pos = viewport->getPositionInViewport(new GVector3(getPositionX(), getPositionY(),0));
	r.top = max(pos.y, 1);
	r.left = max(pos.x,1);
	r.bottom = min(pos.y + _bound.top - _bound.bottom, WINDOW_HEIGHT - 1);
	r.right = min(pos.x	 + _bound.right - _bound.left, WINDOW_WIDTH - 1);

	DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	DeviceManager::getInstance()->getDevice()->StretchRect(
		_surface,
		NULL,
		DeviceManager::getInstance()->getSurface(),
		&r,
		D3DTEXF_NONE
		);
}

void Land::release()
{
	for (auto it : _listComponent)
	{
		SAFE_DELETE(it.second);
	}
	_listComponent.clear();
}

GVector2 Land::getPosition()
{
	return GVector2(_bound.left, _bound.top);
}

float Land::getPositionX()
{
	return _bound.left;
}

float Land::getPositionY()
{
	return _bound.top;
}

void Land::setPosition(GVector3 vector)
{
	_bound.left = vector.x;
	_bound.top = vector.y;
}

void Land::setPosition(float x, float y, float z)
{
	_bound.left = x;
	_bound.top = y;
}

void Land::setPosition(GVector2 position)
{
	_bound.left = position.x;
	_bound.top = position.y;
}

void Land::setPosition(float x, float y)
{
	_bound.left = x;
	_bound.top = y;
}

void Land::setPositionX(float x)
{
	_bound.left = x;
}

void Land::setPositionY(float y)
{
	_bound.right = y;
}

GVector2 Land::getScale()
{
	return VECTOR2ONE;
}

void Land::setScale(GVector2 scale)
{
	// do nothing.
}

void Land::setScale(float scale)
{
	// do nothing.
}

void Land::setScaleX(float sx)
{
	// do nothing.
}

void Land::setScaleY(float sy)
{
	// do nothing.
}

GVector2 Land::getOrigin()
{
	return VECTOR2ZERO;
}

void Land::setOrigin(GVector2 origin)
{
	// do nothing.
}

float Land::getRotate()
{
	return 0.0f;
}

void Land::setRotate(float degree)
{
	// do nothing.
}

void Land::setZIndex(float z)
{
	// do nothing.
}

float Land::getZIndex()
{
	return 1;		// hard code
}

GVector2 Land::getAnchorPoint()
{
	return GVector2(_bound.left, _bound.right);
}

RECT Land::getBounding()
{
	return _bound;
}

eLandType Land::getType()
{
	return _type;
}

void Land::enableJump(bool enable)
{
	if (enable != _canJump)
		_canJump = enable;
}

bool Land::canJump()
{
	return _canJump;
}

Land::~Land()
{
}
