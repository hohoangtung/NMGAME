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
	auto vpBounding = SceneManager::getInstance()->getCurrentScene()->getViewport()->getBounding();

	// kt coi đi tới chưa, chưa tới mới tạo
	if (this->getPositionX() > vpBounding.right && this->getPositionY() < vpBounding.top)
	{
		if (_stopWatch->isStopWatch(_timeCreate))
		{
			if (_number != -1 && _counter < _number)
			{
				_counter++;
				_listObjects.push_back(getObject(_createType));

				if (_counter < _number)
					_stopWatch->restart();
			}
			else if (_number == -1)
			{
				_listObjects.push_back(getObject(_createType));
				_stopWatch->restart();
			}
		}
	}
	else if (this->getPositionX() <= vpBounding.left || this->getPositionY() <= vpBounding.bottom)
	{
		// qua luôn rồi thì hủy
		this->setStatus(eStatus::DESTROY);
		return;
	}

	for (auto object : _listObjects)
	{
		object->update(deltatime);

		if (_direction == -1 && object->getPositionX() < vpBounding.left)
		{
			object->setStatus(eStatus::DESTROY);
		}
		else if (_direction == 1 && object->getPositionX() > vpBounding.right)
		{
			object->setStatus(eStatus::DESTROY);
		}
	}

	this->deleteObject();
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
		auto soldier = new Soldier(eStatus::RUNNING, this->getPosition(), _direction);
		soldier->init();
		auto pos = soldier->getPosition();
		return soldier;
		break;
	}
	default:
		break;
	}
}

void ObjectCreator::deleteObject()
{
	for (auto object : _listObjects)
	{
		if (object->getStatus() == eStatus::DESTROY)
		{
			object->release();
			
			remove(_listObjects.begin(), _listObjects.end(), object);
			_listObjects.pop_back();

			delete object;

			break;
		}
	}
}

RECT ObjectCreator::getBounding()
{
	return RECT{0, 0, 0, 0};
}

vector<BaseObject*> ObjectCreator::getObjects()
{
	return _listObjects;
}
