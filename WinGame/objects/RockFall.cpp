#include "RockFall.h"


RockFall::RockFall(GVector2 position) : BaseEnemy(eID::ROCKFALL)
{
	this->_beginPosition = position;
	this->_tempPosition.x = position.x - 1;
	this->_tempPosition.y = position.y - 1;
}

RockFall::~RockFall()
{

}

// init
void RockFall::init()
{
	_hitpoint = ROCKFALL_HITPOINT;
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::ROCKFALL);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(_beginPosition);
	this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));

	_animation = new Animation(_sprite, 0.07f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "fall_01"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "fall_02"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "fall_03"));
	_animation->stop();

	Movement* movement = new Movement(VECTOR2ZERO, VECTOR2ZERO, _sprite);
	Gravity* gravity = new Gravity(VECTOR2ZERO, movement);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Gravity"] = gravity;
	this->_listComponent["Movement"] = movement;
	this->_listComponent["CollisionBody"] = collisionBody;

	this->setPhysicsBodySide(eDirection::ALL);
	this->setHitpoint(ROCKFALL_HITPOINT);
	this->setStatus(eStatus::HOLDING);

	_stopWatch = new StopWatch();
}

// update
void RockFall::update(float deltatime)
{

	if (this->getStatus() == eStatus::DESTROY)
		return;
	// if - return thì không cần else

	this->checkPosition();

	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}

	if (this->getStatus() == eStatus::BURST)
	{
		auto movement = (Movement*)_listComponent["Movement"];
		movement->setVelocity(VECTOR2ZERO);
		auto gravity = (Gravity*)_listComponent["Gravity"];
		gravity->setGravity(VECTOR2ZERO);
		this->setPhysicsBodySide(eDirection::NONE);
		if (_explosion == nullptr)
		{
			this->_explosion = new Explosion(2);
			_explosion->init();
			_explosion->setScale(SCALE_FACTOR);
			_explosion->setPosition(this->getPosition());
		}
		else
		{
			this->_explosion->update(deltatime);
			if (_explosion->getStatus() == eStatus::DESTROY)
			{
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
	_animation->update(deltatime);
	if (this->getStatus() == eStatus::HOLDING)
	{
		this->moveAround(deltatime);
		if (_stopWatch->isStopWatch(2000.0f))
		{
			this->setPosition(_beginPosition);
			this->setStatus(eStatus::FALLING);
			auto move = (Movement*) this->_listComponent["Movement"];
			move->setVelocity(VECTOR2ZERO);
			auto gravity = (Gravity*)this->_listComponent["Gravity"];
			gravity->setGravity(ROCKFALL_GRAVITY);
		}
		
	}
	
	
	if (this->getStatus() == eStatus::FALLING)
	{
		_animation->start();


	}

	if (this->getStatus() == eStatus::JUMPING)
	{
		_animation->start();
		auto gravity = (Gravity*)this->_listComponent["Gravity"];
		gravity->setStatus(eGravityStatus::FALLING__DOWN);
		gravity->setGravity(ROCKFALL_GRAVITY);

		auto move = (Movement*) this->_listComponent["Movement"];
		move->setVelocity(GVector2(move->getVelocity().x, 100));
		this->setStatus(FALLING);
	}
	

}

// draw
void RockFall::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_explosion != NULL && this->getStatus() == eStatus::BURST)
		_explosion->draw(spriteHandle, viewport);
	if (this->getStatus() == NORMAL || this->getStatus() == FALLING || this->getStatus() == JUMPING || this->getStatus() == HOLDING)
	{
		if (_animation->isAnimate())
			_animation->draw(spriteHandle, viewport);
		else
			_sprite->render(spriteHandle, viewport);
	}
}

// release
void RockFall::release()
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

// getVelocity
GVector2 RockFall::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

// getComponent
IComponent* RockFall::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}

// moveAround: hiệu ứng rung
void RockFall::moveAround(float deltatime)
{
	GVector2 position = this->getPosition();

	if (position.x < _tempPosition.x)
	{
		auto move = (Movement*) this->_listComponent["Movement"];
		auto gravity = (Gravity*)this->_listComponent["Gravity"];

		move->setVelocity(ROCKFALL_HORIZONTAL_VELOC);
		gravity->setGravity(VECTOR2ZERO);
	}

	if (position.x > _tempPosition.x)
	{
		auto move = (Movement*) this->_listComponent["Movement"];
		auto gravity = (Gravity*)this->_listComponent["Gravity"];

		move->setVelocity(ROCKFALL_HORIZONTAL_VELOC_PRE);
		gravity->setGravity(VECTOR2ZERO);
	}
	
}


// checkCollision
float RockFall::checkCollision(BaseObject * object, float dt)
{
	if (this->getStatus() == eStatus::DESTROY)
		return 0.0f;
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::LAND)
	{
		if (((Land*)object)->getType() == eLandType::WATER)
			return 0.0f;

		auto result = find(_passedLand.begin(), _passedLand.end(), object);
		if ((result == _passedLand.end()) || (result._Ptr = nullptr))
		{
			if (collisionBody->checkCollision(object, direction, dt))
			{
				if (this->getVelocity().y > 0)
					return 0.0;
				if (direction == eDirection::TOP)
				{
					auto gravity = (Gravity*)this->_listComponent["Gravity"];
					gravity->setStatus(eGravityStatus::SHALLOWED);
					gravity->setGravity(VECTOR2ZERO);

					auto move = (Movement*) this->_listComponent["Movement"];
					move->setVelocity(VECTOR2ZERO);

					this->setStatus(eStatus::JUMPING);
					_passedLand.push_back(object);

				}

			}
		}
		
	}

	if (objectId == eID::BILL)
	{
		if (this->getStatus() == eStatus::HOLDING)
			return 0.0f;

		if (collisionBody->checkCollision(object, direction, dt,false))
		{
			if (((Bill*)object)->isInStatus(eStatus::DYING) == false)
			{
				((Bill*)object)->setStatus(eStatus::DYING);
				((Bill*)object)->die();
			}
		}
		//this->setStatus(DESTROY);
	}
	

	return 0.0f;
}


// checkPosition
void RockFall::checkPosition()
{
	//if (this->getStatus() != eStatus::NORMAL)
		//return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();
	//if (position.x > screenBound.right)			// thay đổi khi sang map 2
	//{
	//	this->setStatus(eStatus::DESTROY);
	//}
	GVector2 viewportposition = viewport->getPositionWorld();
	if (position.x > screenBound.right || BaseObject::getBounding().top < viewportposition.y - WINDOW_HEIGHT)
	{
		this->setStatus(eStatus::DESTROY);
	}
}

// dropHitpoint
void RockFall::dropHitpoint()
{
	if (this->getStatus() == eStatus::HOLDING)
	{
		return;
	}
	_hitpoint--;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}

void RockFall::dropHitpoint(int damage)
{
	if (this->getStatus() == eStatus::HOLDING)
	{
		return;
	}
	_hitpoint -= damage;
	if (_hitpoint <= 0)
	{
		this->setStatus(eStatus::BURST);
	}
}