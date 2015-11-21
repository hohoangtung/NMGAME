#include "AirCraft.h"

AirCraft::AirCraft(GVector2 pos, GVector2 hVeloc, GVector2 amp, float freq, eAirCraftType type) : BaseObject(eID::AIRCRAFT)
{
	this->_type = type;
	this->_beginPosition = pos;
	this->_horizontalVeloc = hVeloc;
	this->_amplitude = amp;
	this->_frequence = freq;
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
	_animation = new Animation(_sprite, 0.07f);
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_1"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_2"));
	_animation->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "invul_3"));
	_animation->stop();
	
	Movement* movement = new Movement(VECTOR2ZERO, _horizontalVeloc, _sprite);
	SinMovement* sinmovement = new SinMovement(_amplitude, _frequence, _sprite);
	Gravity* gravity = new Gravity(VECTOR2ZERO, movement);
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_listComponent["Movement"] = movement;
	this->_listComponent["Gravity"] = gravity;
	this->_listComponent["Sinmovement"] = sinmovement;
	this->_listComponent["CollisionBody"] = collisionBody;
	this->setPhysicsBodySide(eDirection::NONE);
	_explosion = NULL;
	_explored = false;
	__hook(&InputController::__eventkeyPressed, _input, &AirCraft::keypressed);
}

void AirCraft::updateInput(float deltatime)
{

}
void AirCraft::keypressed(KeyEventArg* keycode) // test
{
	if (keycode->_key == DIK_M)
		this->_status = eStatus::BURST;
}

void AirCraft::initExplosion()
{
	_explosion = new Explosion(2);
	_explosion->init();
	((Explosion*)_explosion)->setPosition(this->_sprite->getPosition());
	Movement* move = (Movement*)getComponent("Movement");
	move->setAccelerate(VECTOR2ZERO);
	move->setVelocity(VECTOR2ZERO);

	SinMovement* sinmove = (SinMovement*)getComponent("Sinmovement");
	sinmove->setAmplitude(VECTOR2ZERO);
	sinmove->setFrequency(0.0f);

}
void AirCraft::updateExplosion(float deltatime)
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
void AirCraft::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}
	
	if (this->_status == eStatus::BURST)
	{
		if (_explosion == NULL)
			initExplosion();
		if (_explosion != NULL)
			updateExplosion(deltatime);
	}
	_animation->update(deltatime);
	if (this->_status == eStatus::EXPLORING)
	{
		auto bill = ((PlayScene*) SceneManager::getInstance()->getCurrentScene())->getBill();
		if (isRectangleIntersected(bill->getBounding(), this->getBounding()))
		{
			this->setStatus(eStatus::DESTROY);
			bill->changeBulletType(this->_type);
			return;
		}
		switch (_type)
		{
		case B:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "b_ammo"));
			break;
		case F:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "f_ammo"));
			break;
		case L:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "l_ammo"));
			break;
		case M:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "m_ammo"));
			break;
		case R:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "r_ammo"));
			break;
		case S:
			this->_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(this->_id, "s_ammo"));
			break;
		default:
			_animation->start();
			break;
		}
		Movement* move = (Movement*)getComponent("Movement");
		move->setVelocity(AIRCRAFT_FORCE);

		Gravity* gravity = (Gravity*)getComponent("Gravity");
		gravity->setGravity(AIRCRAFT_GRAVITY);

		//this->setPhysicsBodySide((eDirection)ALL_EDGES);
		
		this->setStatus(eStatus::EXPLORED);
	}

}

eAirCraftType AirCraft::getType()
{
	return _type;
}

void AirCraft::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if ((_status & (NORMAL | EXPLORING | EXPLORED)) ==_status)
	{
		if (_animation->isAnimate())
			_animation->draw(spriteHandle, viewport);
		else
			this->_sprite->render(spriteHandle, viewport);
	}
	if (this->_status == eStatus::BURST)
	{
		if (_explosion != NULL)
			_explosion->draw(spriteHandle, viewport);
	}
	if (viewport->isContains(this->getBounding()) == false)
	{
		this->setStatus(eStatus::DESTROY);
	}
}
void AirCraft::release()
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

IComponent* AirCraft::getComponent(string componentName)
{
	auto it = _listComponent.find(componentName);
	if (it == _listComponent.end())
		throw new exception("key not match");
	return it->second;
}


float AirCraft::checkCollision(BaseObject * object, float dt)
{
	if (this->_status == eStatus::NORMAL)
		return 0.0f;
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];
	auto objeciId = object->getId();
	eDirection direction;

	if (collisionBody->checkCollision(object, direction, dt))
	{
		if (objeciId == eID::BOX || objeciId == eID::BRIDGE)		// => ??
		{
			if (direction == eDirection::TOP)
			{
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);
				gravity->setGravity(VECTOR2ZERO);

				auto move = (Movement*) this->_listComponent["Movement"];
				move->setVelocity(VECTOR2ZERO);
			}
			else if (direction & (LEFT | RIGHT) == direction)
			{
				auto move = (Movement*) this->_listComponent["Movement"];
				GVector2 veloc = move->getVelocity();
				veloc.x *= -1;
				move->setVelocity(veloc);
			}
		}
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