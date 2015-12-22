#include "RockFly.h"



RockFly::RockFly(GVector2 leftPos, GVector2 rightPos) : BaseObject(eID::ROCKFLY)
{
	this->_leftPosition = leftPos;
	this->_rightPosition = rightPos;
}

RockFly::~RockFly()
{

}

// init
void RockFly::init()
{


	this->_sprite = SpriteManager::getInstance()->getSprite(eID::ROCKFLY);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(_rightPosition);
	this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::ROCKFLY, "normal"));

	Movement* movement = new Movement(VECTOR2ZERO, ROCKFLY_HORIZONTAL_VELOC, _sprite);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setStatus(NORMAL);
	this->setPhysicsBodySide(eDirection::TOP);


}

// update
void RockFly::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;

	this->checkPosition();

	
	if (this->getStatus() == eStatus::WAITING)
	{
		if (true) // 7ung - tesst
		{
			auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();

			bill->setPositionX(this->getPositionX());
			if (bill->isInStatus(MOVING_LEFT) || bill->isInStatus(MOVING_RIGHT))
				this->setStatus(RUNNING);
		}

	
	}

	GVector2 position = this->getPosition();
	if (position.x < _leftPosition.x)
	{
		auto move = (Movement*) this->_listComponent["Movement"];
		move->setVelocity(ROCKFLY_HORIZONTAL_VELOC);
	}

	if (position.x > _rightPosition.x)
	{
		auto move = (Movement*) this->_listComponent["Movement"];
		move->setVelocity(ROCKFLY_HORIZONTAL_VELOC_PRE);
	}

	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}

	
}

// draw
void RockFly::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	//if (this->getStatus() == eStatus::NORMAL || this->getStatus() == eStatus::WAITING)
	_sprite->render(spriteHandle, viewport);
}

// release
void RockFly::release()
{
	for (auto component : _listComponent)
	{
		delete component.second;
	}
	_listComponent.clear();
	
	SAFE_DELETE(this->_sprite);
}

// getVelocity
GVector2 RockFly::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

// getComponent
IComponent* RockFly::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}


// checkPosition
void RockFly::checkPosition()
{
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();
	GVector2 viewportposition = viewport->getPositionWorld();
	if (position.x > screenBound.right || BaseObject::getBounding().top < viewportposition.y - WINDOW_HEIGHT)
	{
		this->setStatus(eStatus::DESTROY);
	}
	if (isRectangleIntersected(bill->getBounding(), this->getBounding()))
	{
		// Nếu giao nhau
		//return;  // do nothing
		if (position.y < bill->getPositionY())
		{
			// Nếu y của cục đá nhỏ hơn y của thằng bill thì set wait ???
			this->setStatus(eStatus::WAITING);
		}
		if (bill->isInStatus(MOVING_LEFT) || bill->isInStatus(MOVING_RIGHT) || bill->isInStatus(eStatus::JUMPING) || bill->isInStatus(eStatus::FALLING))
		{
			this->setStatus(NORMAL);
		}
	}
	else
	{
		if (this->getStatus() != eStatus::DESTROY)
		{
			this->setStatus(NORMAL);
		}
	}

	
}

RECT RockFly::getBounding()
{
	RECT basebound = BaseObject::getBounding();
	basebound.top -= 5 * this->getScale().y;
	basebound.bottom += 7 * this->getScale().y;
	return  basebound;
}
