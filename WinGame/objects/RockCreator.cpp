
#include "RockCreator.h"

RockCreator::RockCreator(GVector2 position) : BaseObject(eID::ROCKCREATOR)
{
	this->_position = position;
}

RockCreator::~RockCreator()
{
}

void RockCreator::init()
{
	_stopwatch = new StopWatch();
	this->setStatus(eStatus::WAITING);
}


void RockCreator::update(float deltatime)
{
	auto viewport = ((Stage3*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	GVector2 viewportposition = viewport->getPositionWorld();
	if (_position.y < viewportposition.y)
	{
		this->setStatus(eStatus::NORMAL);
	}
	if (this->getStatus() == eStatus::WAITING || this->getStatus() == eStatus::DESTROY)
		return;
	if (this->_rockInstance == nullptr)
	{
		if (_stopwatch->isStopWatch(700.0f))
		{
			_rockInstance = new RockFall(this->_position);
			_rockInstance->init();
		}
	}
	else
	{
		_rockInstance->update(deltatime);
		if (_rockInstance->getStatus() == eStatus::DESTROY)
		{
			_rockInstance->release();
			delete _rockInstance;
			_rockInstance = nullptr;
			this->_stopwatch->restart();
		}
	}
}


void RockCreator::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_rockInstance == nullptr)
		return;
	if (this->getStatus() == eStatus::WAITING || this->getStatus() == eStatus::DESTROY)
		return;
	_rockInstance->draw(spriteHandle, viewport);
}


void RockCreator::release()
{
	if (_rockInstance == nullptr)
		return;
	_rockInstance->release();
	delete _rockInstance;
	_rockInstance = nullptr;

	SAFE_DELETE(_stopwatch);
}

float RockCreator::checkCollision(BaseObject * object, float dt)
{
	if (_rockInstance == nullptr)
		return 0.0f;
	auto otherid = object->getId();
	if (otherid != eID::LAND && otherid != eID::BILL)
		return 0.0f;
	_rockInstance->checkCollision(object, dt);

	return 0.0f;
}

BaseObject* RockCreator::getRock()
{
	return _rockInstance;
}

RECT RockCreator::getBounding()
{
	return RECT();
}

