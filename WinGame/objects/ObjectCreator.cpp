#include "ObjectCreator.h"

ObjectCreator::ObjectCreator(GVector2 position, int width, int height, eID type, int direction, float timeCreate, int number)
	: EmptyObject(eID::CREATOR, position, width, height)
{
	_createType = type;
	_timeCreate = timeCreate;
	_number = number;

	_stopWatch = new StopWatch();

	_direction = direction;
}

ObjectCreator::~ObjectCreator()
{
}

void ObjectCreator::init()
{

}

void ObjectCreator::update(float deltatime)
{
	if (_stopWatch->isStopWatch(_timeCreate))
	{
		if (_number != -1 && _counter < _number)
		{
			_counter++;
			_listObjects.push_back(getObject(_createType));
		}
		else if (_number == -1)
		{
			_listObjects.push_back(getObject(_createType));
		}

		_stopWatch->restart();
	}

	for (auto object : _listObjects)
	{
		object->update(deltatime);
	}
}

void ObjectCreator::draw(LPD3DXSPRITE spriteHandle, Viewport * viewport)
{
	for (auto object : _listObjects)
	{
		object->draw(spriteHandle, viewport);
	}
}

void ObjectCreator::release()
{
	SAFE_DELETE(_stopWatch);

	for (auto object : _listObjects)
	{
		SAFE_DELETE(object);
	}

	_listObjects.clear();
}

float ObjectCreator::checkCollision(BaseObject * object, float dt)
{
	for (auto item : _listObjects)
	{
		item->checkCollision(object, dt);
	}

	return 0.0f;
}

BaseObject * ObjectCreator::getObject(eID id)
{
	switch (id)
	{
	case SOLDIER:
	{
		auto soldier = new Soldier(eStatus::JUMPING, this->getPosition(), _direction);
		soldier->init();
		return soldier;
		break;
	}
	default:
		break;
	}
}
