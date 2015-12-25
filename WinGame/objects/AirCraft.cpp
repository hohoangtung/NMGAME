#include "AirCraft.h"

AirCraft::AirCraft(GVector2 pos, GVector2 hVeloc, GVector2 amp, float freq, eAirCraftType type) : BaseObject(eID::AIRCRAFT)
{
	this->_type = type;
	this->_beginPosition = pos;
	this->_horizontalVeloc = hVeloc;
	this->_amplitude = amp;
	this->_frequence = freq;
	if (this->_amplitude.x > 0)
	{
		this->_verticalflag = true;
	}
	else
	{
		this->_verticalflag = false;
	}
}

AirCraft::~AirCraft()
{
}

void AirCraft::init()
{
	this->_sprite = SpriteManager::getInstance()->getSprite(eID::AIRCRAFT);
	this->_sprite->setScale(SCALE_FACTOR);
	this->_sprite->setPosition(this->_beginPosition);
	this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "normal"));
	
	Movement* movement = new Movement(VECTOR2ZERO, _horizontalVeloc, _sprite);
	SinMovement* sinmovement = new SinMovement(_amplitude, _frequence, _sprite);
	CollisionBody* collisionBody = new CollisionBody(this);

	this->_listComponent["Movement"] = movement;
	this->_listComponent["Sinmovement"] = sinmovement;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::NONE);
	this->setStatus(eStatus::HIDING);
	_explosion = nullptr;
	_item = nullptr;
	_explored = false;
}

void AirCraft::initExplosion()
{
	_explosion = new Explosion(2);
	_explosion->init();
	((Explosion*)_explosion)->setPosition(this->_sprite->getPosition());
	Movement* move = (Movement*)this->getComponent("Movement");
	move->setAccelerate(VECTOR2ZERO);
	move->setVelocity(VECTOR2ZERO);

	SinMovement* sinmove = (SinMovement*)this->getComponent("Sinmovement");
	sinmove->setAmplitude(VECTOR2ZERO);
	sinmove->setFrequency(0.0f);

}

void AirCraft::checkifOutofScreen()
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();

	if (position.x > screenBound.right)
	{
		this->setStatus(eStatus::DESTROY);
	}
}

void AirCraft::updateHiding()
{
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	if (this->_verticalflag)
	{
		if (this->getPositionY() < viewport->getPositionWorld().y - WINDOW_HEIGHT)
		{
			this->setStatus(eStatus::NORMAL);
		}
	}
	else
	{
		if (this->getBounding().right < screenBound.left)
		{
			this->setStatus(eStatus::NORMAL);
		}
	}
}

void AirCraft::updateExploring(float deltatime)
{
	auto bill = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getBill();
	if (isRectangleIntersected(bill->getBounding(), this->getBounding()))
	{
		this->setStatus(eStatus::DESTROY);
		bill->changeBulletType(this->_type);
		return;
	}

	Movement* move = (Movement*)getComponent("Movement");
	move->setVelocity(VECTOR2ZERO);
}
void AirCraft::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::HIDING)
	{
		this->updateHiding();
		return;
	}
	else
	{
		this->checkifOutofScreen();

		for (auto component : _listComponent)
		{
			component.second->update(deltatime);
		}
	}

	if (this->_status == eStatus::BURST)
	{
		if (_explosion == nullptr)
			initExplosion();
		_item = new Item(this->_sprite->getPosition(), this->_type);
		_item->init();
		this->setStatus(eStatus::EXPLORING);
	}
	if (this->_status == eStatus::EXPLORING)
	{
		this->updateExploring(deltatime);
		this->setStatus(eStatus::EXPLORED);
	}

	if (_explosion != nullptr)
		_explosion->update(deltatime);
	if (_item != nullptr)
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

eAirCraftType AirCraft::getType()
{
	return _type;
}

void AirCraft::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY || this->getStatus() == eStatus::HIDING)
		return;

	if (this->getStatus() == eStatus::NORMAL)
		_sprite->render(spriteHandle, viewport);
	if (_explosion != nullptr)
		_explosion->draw(spriteHandle, viewport);
	if (_item != nullptr)
		_item->draw(spriteHandle, viewport);

}
void AirCraft::release()
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
	SAFE_DELETE(this->_sprite);
}

IComponent* AirCraft::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}

float AirCraft::checkCollision(BaseObject * object, float dt)
{
	if (_item != nullptr)
	{
		_item->checkCollision(object, dt);
	}
	return 0.0f;
}

GVector2 AirCraft::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

void AirCraft::setExplored()
{
	this->_explored = true;
}