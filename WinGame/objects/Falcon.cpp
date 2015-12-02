#include "Falcon.h"




Falcon::Falcon(GVector2 position, GVector2	force, eAirCraftType type) : BaseObject(eID::FALCON)
{
	this->_type = type;
	this->_beginPosition = position;
	this->_force = force;

}

Falcon::Falcon(float x, float y, GVector2	force, eAirCraftType type) : BaseObject(eID::FALCON)
{
	
	this->_type = type;
	this->_beginPosition = GVector2(x, y);
	this->_force = force;

}

Falcon::~Falcon() {

};

void Falcon::init()
{
	

	this->_sprite = SpriteManager::getInstance()->getSprite(eID::FALCON);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(this->_beginPosition);

	_animations = new Animation(_sprite, 0.5f);
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "open_01"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "open_02"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "open_03"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "opened_01"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "opened_02"));
	_animations->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "opened_03"));



	Movement* movement = new Movement(VECTOR2ZERO, HORIZONTAL_VELOC, _sprite);
	Gravity* gravity = new Gravity(VECTOR2ZERO, movement);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["Gravity"] = gravity;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::NONE);

	_explored = false;
	_explosion = NULL;
}

void Falcon::init2()
{
	// Lấy sprite của AirCraft

	this->_sprite = SpriteManager::getInstance()->getSprite(eID::AIRCRAFT);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(this->_beginPosition);
	
	Movement* movement = new Movement(VECTOR2ZERO, HORIZONTAL_VELOC, _sprite);
	Gravity* gravity = new Gravity(VECTOR2ZERO, movement);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["Gravity"] = gravity;
}
void Falcon::initExplosion()
{

	_explosion = new Explosion(2);
	_explosion->init();
	((Explosion*)_explosion)->setPosition(this->_sprite->getPosition());

	Movement* move = (Movement*)getComponent("Movement");
	move->setAccelerate(VECTOR2ZERO);
	move->setVelocity(VECTOR2ZERO);

}

void Falcon::updateExplosion(float deltatime)
{
	_explosion->update(deltatime);
	if (this->_explosion->getStatus() == eStatus::DESTROY)
	{
		if (this->_explored == true)
			this->setStatus(eStatus::DESTROY);
		else
			this->setStatus(eStatus::EXPLORING);
	}
}



void Falcon::update(float deltatime)
{

	if (this->getStatus() == eStatus::DESTROY)
		return;

	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}

	
	if (this->_status == eStatus::NORMAL)
	{
		if (this->_animations->getIndex() == 0)
			this->setStatus(eStatus::HIDDEN);
	}
	if (this->_status == eStatus::HIDDEN)
	{
		if (this->_animations->getIndex() != 0)
			this->setStatus(eStatus::NORMAL);
	}

	if (this->_status == eStatus::BURST)
	{
		if (_explosion == NULL)
			initExplosion();
		if (_explosion != NULL)
			updateExplosion(deltatime);
	}
	_animations->update(deltatime);

	if (this->_status == eStatus::EXPLORING)
	{
		_animations->stop();
		this->init2();
		auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
		if (isRectangleIntersected(bill->getBounding(), this->getBounding()))
		{
			this->setStatus(eStatus::DESTROY);
			bill->changeBulletType(this->_type);
			return;
		}
		
		switch (_type)
		{
		case B:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "b_ammo"));
			break;
		case F:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "f_ammo"));
			break;
		case L:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "l_ammo"));
			break;
		case M:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "m_ammo"));
			break;
		case R:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "r_ammo"));
			break;
		case S:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "s_ammo"));
			break;
		default:
			//_animations->start();
			break;
		}

		Movement* move = (Movement*)getComponent("Movement");
		move->setVelocity(_force);

		Gravity* gravity = (Gravity*)getComponent("Gravity");
		gravity->setGravity(FALCON_GRAVITY);

		this->setStatus(eStatus::EXPLORED);
	}

}


void Falcon::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{

	if (this->getStatus() == eStatus::DESTROY)
		return;

	if ((_status & (NORMAL | EXPLORING | EXPLORED | HIDDEN)) == _status)
	{
		if (_animations->isAnimate())
			_animations->draw(spriteHandle, viewport);
		else
			this->_sprite->render(spriteHandle, viewport);
	}
	if (this->_status == eStatus::BURST)
	{
		if (_explosion != NULL)
			_explosion->draw(spriteHandle, viewport);
	}
	/*if (viewport->isContains(this->getBounding()) == false)
	{
		this->setStatus(eStatus::DESTROY);
	}*/
}

void Falcon::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	if (this->_explosion != NULL)
		this->_explosion->release();
	SAFE_DELETE(this->_explosion);
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
	if (this->_status == eStatus::NORMAL)
		return 0.0f;
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	auto objeciId = object->getId();
	eDirection direction;

	if (collisionBody->checkCollision(object, direction, dt))
	{
		if (objeciId == eID::LAND || objeciId == eID::BRIDGE)	
		{
			if (direction == eDirection::TOP)
			{
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);
				gravity->setGravity(VECTOR2ZERO);

				auto move = (Movement*) this->_listComponent["Movement"];
				move->setVelocity(VECTOR2ZERO);
			}
		}
	}


	return 0.0f;
}

GVector2 Falcon::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
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

void Falcon::setExplored()
{
	this->_explored = true;
}

bool Falcon::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}

void Falcon::onCollisionBegin(CollisionEventArg* collision_event)
{

}

void Falcon::onCollisionEnd(CollisionEventArg* collision_event)
{

}