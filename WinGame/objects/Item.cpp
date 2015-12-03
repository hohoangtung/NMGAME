#include "Item.h"
RECT Item::getFrameRectFromType(eAirCraftType type)
{
	switch (type)
	{
	case B:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "b_ammo");
		break;
	case F:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "f_ammo");
		break;
	case L:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "l_ammo");
		break;
	case M:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "m_ammo");
		break;
	case R:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "r_ammo");
		break;
	case S:
		return SpriteManager::getInstance()->getSourceRect(eID::AIRCRAFT, "s_ammo");
		break;
	case I:
		break;
	default:
		break;
	}

}

Item::Item(GVector2 position, eAirCraftType type) : BaseObject(eID::ITEM)
{
	_startposition = position;
	_type = type;
}

float Item::checkCollision(BaseObject* object, float dt)
{
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	auto objeciId = object->getId();
	eDirection direction;

	if (collisionBody->checkCollision(object, direction, dt))
	{
		if (objeciId == eID::LAND || objeciId == eID::BRIDGE)		// => ??
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
			}
		}
		if (objeciId == eID::BILL)
		{
			this->setStatus(eStatus::DESTROY);
			((Bill*)object)->changeBulletType(this->_type);
		}
	}
	return 0.0f;
}



void Item::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::AIRCRAFT);
	_sprite->setFrameRect(Item::getFrameRectFromType(_type));
	_sprite->setScale(SCALE_FACTOR);
	_sprite->setPosition(_startposition);

	if (this->_type == eAirCraftType::I)
	{
		_animation = new Animation(_sprite, 0.07f);
		_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_1"));
		_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_2"));
		_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_3"));
	}


	Movement* movement = new Movement(VECTOR2ZERO, ITEM_FORCE, _sprite);
	Gravity* gravity = new Gravity(ITEM_GRAVITY, movement);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["Gravity"] = gravity;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::ALL);
}

void Item::update(float deltatime)
{
	if (this->_type == eAirCraftType::I)
	{
		_animation->update(deltatime);
	}
	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}
}

void Item::checkifOutofScreen()
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	auto viewport = ((PlayScene*)SceneManager::getInstance()->getCurrentScene())->getViewport();
	RECT screenBound = viewport->getBounding();
	GVector2 position = this->getPosition();
	if (position.y < screenBound.bottom)
	{
		this->setStatus(eStatus::DESTROY);
	}
}

void Item::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->_type == eAirCraftType::I)
		_animation->draw(spriteHandle, viewport);
	else
		_sprite->render(spriteHandle, viewport);
}

void Item::release()
{
	for (auto component : _listComponent)
	{
		SAFE_DELETE(component.second);
	}
	_listComponent.clear();
	SAFE_DELETE(_sprite);
	SAFE_DELETE(_animation);
}

RECT Item::getBounding()
{
	RECT baseBound = BaseObject::getBounding();
	baseBound.left += (7 + this->getScale().x);
	baseBound.right -= (7 - this->getScale().y);
	return baseBound;
}

GVector2 Item::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

Item::~Item()
{
}