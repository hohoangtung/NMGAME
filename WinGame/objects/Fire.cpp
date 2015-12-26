#include "Fire.h"


Fire::Fire(GVector2 pos) :BaseObject(eID::FIRE)
{
	_startposition = pos;
}

void Fire::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::FIRE);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(_startposition);
	this->setStatus(eStatus::NORMAL);
	_singleFire1 = new SingleFire(this->getPosition(),1);
	_singleFire1->init();
	_singleFire2 = new SingleFire(this->getPosition(), -1);
	_singleFire2->init();
}
void Fire::update(float deltatime)
{
	eStatus n = this->getStatus();
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (_singleFire1 != nullptr)
		_singleFire1->update(deltatime);
	if (_singleFire2 != nullptr)
		_singleFire2->update(deltatime);
	for (auto it = _listComponent.begin(); it != _listComponent.end(); it++)
	{
		it->second->update(deltatime);
	}
}
void Fire::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (_singleFire1 != nullptr)
		_singleFire1->draw(spriteHandle, viewport);
	if (_singleFire2 != nullptr)
		_singleFire2->draw(spriteHandle, viewport);
}
void Fire::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	if (_singleFire1 != nullptr)
	{
		_singleFire1->release();
		delete _singleFire1;
		_singleFire1 = nullptr;
	}
	if (_singleFire2 != nullptr)
	{
		_singleFire2->release();
		delete _singleFire2;
		_singleFire2 = nullptr;
	}
	
}
float Fire::checkCollision(BaseObject* object, float dt)
{
	_singleFire1->checkCollision(object, dt);
	_singleFire2->checkCollision(object, dt);
	return 0.0f;
}
void Fire::checkIfOutofScreen()
{
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	RECT thisBound = this->getBounding();
	GVector2 position = this->getPosition();
	if (thisBound.right < screenBound.left)
	{
		this->setStatus(eStatus::DESTROY);
	}
}
IComponent* Fire::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}
Fire::~Fire()
{
}
Fire::SingleFire::SingleFire(GVector2 startposition,  int veloc) :BaseObject(eID::FIRE)
{
	_startposition = startposition;
	this->_veloc = veloc;
	
}
void Fire::SingleFire::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::FIRE);
	_animation = new Animation(_sprite, 0.1f);
	this->setScale(SCALE_FACTOR);
	this->setPosition(_startposition);
	_animation->addFrameRect(eID::FIRE, "normal","normal_01", NULL);

	Movement* movement = new Movement(VECTOR2ZERO, HORIZONTAL_VELOC, _sprite);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["CollisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Fire::SingleFire::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Fire::SingleFire::onCollisionEnd);
	this->setStatus(NORMAL);
	/*this->setPhysicsBodySide(eDirection::ALL);*/
	
}
void Fire::SingleFire::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	auto pos = this->getPosition();

	auto move = (Movement*) this->_listComponent["Movement"];
	if (_veloc == 1)
	{
		move->setVelocity(HORIZONTAL_VELOC);
	}
	else
	{
		move->setVelocity(HORIZONTAL_VELOC_PRE);
	}
	if (pos.x < (_startposition.x-RANGEMOVE_FIRE/2))
	{
		move->setVelocity(HORIZONTAL_VELOC);
		_veloc = 1;
	}
	if (pos.x >= (_startposition.x+RANGEMOVE_FIRE/2))
	{
		move->setVelocity(HORIZONTAL_VELOC_PRE);
		_veloc = -1;
	}
	
	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}
	_animation->update(deltatime);
}
void Fire::SingleFire::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	
	if (this->getStatus() == eStatus::DESTROY)
		return;
	
	this->_sprite->render(spriteHandle, viewport);
	_animation->draw(spriteHandle, viewport);
}
void Fire::SingleFire::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();

	SAFE_DELETE(this->_sprite);
}

Fire::SingleFire::~SingleFire()
{
}
GVector2 Fire::SingleFire::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}
void Fire::SingleFire::onCollisionBegin(CollisionEventArg* collision_event)
{
	eID objectID = collision_event->_otherObject->getId();
	switch (objectID)
	{
	case eID::BILL:
	{
		if (collision_event->_otherObject->isInStatus(eStatus::DYING) == false)
		{
			collision_event->_otherObject->setStatus(eStatus::DYING);
			((Bill*)collision_event->_otherObject)->die();
		}
		break;
	}
	default:
		break;
	}
}
void Fire::SingleFire::onCollisionEnd(CollisionEventArg* collision_event)
{}
float Fire::SingleFire::checkCollision(BaseObject* object, float dt)
{
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	if (objectId == eID::BILL)
		collisionBody->checkCollision(object, dt,false);
	return 0.0f;
}
IComponent* Fire::SingleFire::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}

RECT Fire::SingleFire::getBounding()
{
	RECT basebound = BaseObject::getBounding();
	basebound.top -= 2 * this->getScale().y;
	basebound.left += 2 * this->getScale().x;
	basebound.left -= 2 * this->getScale().x;
	return basebound;
}