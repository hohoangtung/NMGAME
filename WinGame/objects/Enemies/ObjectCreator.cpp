#include "ObjectCreator.h"

ObjectCreator::ObjectCreator(GVector2 position, int width, int height, eID type, int direction, float timeCreate, int number)
	: EmptyObject(eID::CREATOR, position, width, height)
{
	_createType = type;
	_timeCreate = timeCreate;
	_number = number;

	_stopWatch = new StopWatch();

	_direction = direction;
	_isOnePerOne = false;

	_maxObject = 2;

	_mapType = eMapType::HORIZONTAL;
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
	if ((_mapType == eMapType::HORIZONTAL && this->getPositionX() > vpBounding.right) ||
		(_mapType == eMapType::VERTICAL && this->getPositionY() < vpBounding.top))
	{
		//if (_isOnePerOne == false)
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
					if(_listObjects.size() < _maxObject)
						_listObjects.push_back(getObject(_createType));

					_stopWatch->restart();
				}
			}
		}
		//else
		//{
		//	if (_listObjects.size() == 0)
		//	{
		//		_listObjects.push_back(getObject(_createType));
		//	}
		//}
	}
	else if (this->getPositionX() <= vpBounding.left || this->getPositionY() <= vpBounding.bottom)
	{
		// Tùng: trong màn hình rồi thì huỷ luôn. tại cũng hết giá trị lợi dụng
		// qua luôn rồi thì hủy
		this->setStatus(eStatus::DESTROY);
		return;
	}

	for (auto object : _listObjects)
	{
		object->update(deltatime);

		if (object->getScale().x > 0 && object->getPositionX() < vpBounding.left)
		{
			object->setStatus(eStatus::DESTROY);
		}
		else if (object->getScale().x < 0 && object->getPositionX() > vpBounding.right)
		{
			object->setStatus(eStatus::DESTROY);
		}
		else if (object->getPositionY() > vpBounding.top || object->getPositionY() < vpBounding.bottom)
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
	//if (dynamic_cast<BaseEnemy*>(object))
	//	return 0.0f;
	//Tùng: sửa lại chỉ cho kiểm tra va chạm với land
	// có đoạn sau rồi thì không cần đoạn trên nữa. dynamic cast gây lãng phí nhiều hơn 
	// ref "dynamic_cast is slow": http://stackoverflow.com/questions/4050901/performance-of-dynamic-cast
	if (object->getId() != eID::LAND && object->getId() != eID::BILL)
	{
		return 0.0f;
	}

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
	return RECT();
}

vector<BaseObject*> ObjectCreator::getObjects()
{
	return _listObjects;
}

void ObjectCreator::setOnePerOne(bool enable)
{
	_isOnePerOne = enable;
}

bool ObjectCreator::isOnePerOne()
{
	return _isOnePerOne;
}

void ObjectCreator::setMaxNumber(int number)
{
	_maxObject = number;
}

int ObjectCreator::getMaxNumber()
{
	return _maxObject;
}

void ObjectCreator::setMapType(eMapType type)
{
	_mapType = type;
}

eMapType ObjectCreator::getMapType()
{
	return _mapType;
}

