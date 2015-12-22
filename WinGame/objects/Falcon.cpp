#include "Falcon.h"




Falcon::Falcon(GVector2 position, eAirCraftType type) : BaseObject(eID::FALCON)
{
	this->_type = type;
	this->_beginPosition = position;

}

Falcon::Falcon(float x, float y, eAirCraftType type) : BaseObject(eID::FALCON)
{
	this->_type = type;
	this->_beginPosition = GVector2(x, y);

}

Falcon::~Falcon() {

};

void Falcon::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::FALCON);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(this->_beginPosition);

	_animations = new Animation(_sprite, 0.12f);

	_animations->addFrameRect(eID::FALCON, "normal", "normal", "normal", "normal", "normal", "normal",
		"open_01", "open_02", "open_03", 
		"opened_01", "opened_02", "opened_03", "opened_02", "opened_03", "opened_02", "opened_01", 
		"open_01", NULL);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::NONE);

	_explosion = nullptr;
	_item = nullptr;
}

void Falcon::initExplosion()
{
	_explosion = new Explosion(2);
	_explosion->init();
	((Explosion*)_explosion)->setPosition(this->_sprite->getPosition());

}
void Falcon::checkifOutofScreen()
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	RECT thisBound = this->getBounding();
	GVector2 position = this->getPosition();
	if (thisBound.right < screenBound.left)
	{
		this->setStatus(eStatus::DESTROY);
	}
}

void Falcon::update(float deltatime)
{
	checkifOutofScreen();
	auto status = this->getStatus();

	switch (status)
	{
	case DESTROY:
		return;
	case NORMAL:
	{
		Viewport* viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
		RECT screenBound = viewport->getBounding();
		GVector2 viewportposition = viewport->getPositionWorld();
		if (BaseObject::getBounding().right > screenBound.right || BaseObject::getBounding().top < viewportposition.y - WINDOW_HEIGHT)
		{
			break;
		}
		if (this->getBounding().right > screenBound.right)
		{
			break;
		}
		_animations->update(deltatime);
		break;
	}
	case BURST:
		if (_explosion == NULL)
			initExplosion();
		_item = new Item(this->_sprite->getPosition(), this->_type);
		_item->init();
		this->setStatus(eStatus::EXPLORED);
	default:
		break;
	}

	if (_explosion != NULL)
		_explosion->update(deltatime);
	if (_item != NULL)
	{
		_item->update(deltatime);
		if (_item->getStatus() == eStatus::DESTROY && _explosion->getStatus() == eStatus::DESTROY)
		{
			_item->release();
			delete _item;
			_item = nullptr;
			this->setStatus(eStatus::DESTROY);
		}
	}
}


void Falcon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{

	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::HIDING)
		return;

	if (this->getStatus() == eStatus::NORMAL)
		_animations->draw(spriteHandle, viewport);
	if (_explosion != nullptr)
		_explosion->draw(spriteHandle, viewport);
	if (_item != nullptr)
		_item->draw(spriteHandle, viewport);


}

void Falcon::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	if (_explosion != NULL)
	{
		_explosion->release();
		delete _explosion;
		_explosion = nullptr;
	}
	if (this->_item != NULL)
	{
		_item->release();
		delete _item;
		_item = nullptr;
	}
	SAFE_DELETE(this->_animations);
	SAFE_DELETE(this->_sprite);
}


IComponent* Falcon::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}

float Falcon::checkCollision(BaseObject * object, float dt)
{
	if (_item != nullptr)
	{
		_item->checkCollision(object, dt);
	}
	return 0.0f;
}

bool Falcon::isOpenned()
{
	int index = _animations->getIndex();
	if (index >= 9 && index <= 15)
		return true;
	return false;
}

RECT Falcon::getBounding()
{
	auto baseBound = BaseObject::getBounding();
	baseBound.left += 7 * this->getScale().x;
	baseBound.right -= 7 * this->getScale().x;
	baseBound.top -= 7 * this->getScale().y;
	baseBound.bottom += 7 * this->getScale().y;
	return baseBound;
}


void Falcon::setStatus(eStatus status)
{
	if (_status != status)
		_status = status;
}

eAirCraftType Falcon::getType()
{
	return _type;
}

bool Falcon::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}
