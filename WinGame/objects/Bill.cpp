#include "Bill.h"

Bill::Bill() : BaseObject(eID::BILL)
{
}

Bill::~Bill()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_animations.clear();

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	_componentList.clear();

	SAFE_DELETE(_sprite);
}

void Bill::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Bill::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Bill::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);
	
	this->setPhysicsBodyType(ePhysicsBody::MAN);				// set kiểu của object hiện tại
	auto collisionBody = new CollisionBody(this);
	collisionBody->setPhysicsObjects(ePhysicsBody::LAND);		// set kiểu mà nó va chạm vật lý dừng lại, mặc định ko dừng lại nhưng vẫn báo begin/end

	_componentList["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bill::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Bill::onCollisionEnd);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::BILL, "normal_01", NULL);
	
	_animations[eStatus::NORMAL | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL | eStatus::SHOOTING]->addFrameRect(eID::BILL, "normal_01", "normal_02", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::BILL, "run_01", "run_02", "run_03", "run_04", "run_05", "run_06", NULL);

	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::BILL, "jump_01", "jump_02", "jump_03", NULL);

	_animations[eStatus::LAYING_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN]->addFrameRect(eID::BILL, "lay_down_01", NULL);

	_animations[eStatus::LOOKING_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP]->addFrameRect(eID::BILL, "shot_up_01", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING]->addFrameRect(eID::BILL, "shot_up_01", "shot_up_02" , NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_up_01", "run_shot_up_02", "run_shot_up_03", NULL);

	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_down_01", "run_shot_down_02", "run_shot_down_03", NULL);

	_animations[eStatus::SHOOTING | eStatus::RUNNING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SHOOTING | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_01", "run_shot_02", "run_shot_03", "run_shot_01", "run_shot_02", "run_shot_03", NULL);

	_sprite->drawBounding(false);
	this->setOrigin(GVector2(0.5f, 0.0f));

	_sideCollide = false;
	_movingSpeed = BILL_MOVE_SPEED;

	this->setStatus(eStatus::FALLING);
}

void Bill::update(float deltatime)
{
	if (this->getPositionY() < TEST_LAND)
	{
		this->setPositionY(TEST_LAND);

		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);

		this->standing();
	}

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimateIndex]->update(deltatime);

	// update component để sau cùng để sửa bên trên sau đó nó cập nhật đúng
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

}

void Bill::updateInput(float dt)
{
}

void Bill::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animations[_currentAnimateIndex]->draw(spriteHandle, viewport);
}

void Bill::release()
{
	//_sprite->release();
	_animations.clear();
}

void Bill::onKeyPressed(KeyEventArg* key_event)
{
	switch (key_event->_key)
	{
	case DIK_Z:
	{
		this->jump();
		break;
	}
	case DIK_LEFT:
	{
		this->removeStatus(eStatus::LAYING_DOWN);
		this->removeStatus(eStatus::MOVING_RIGHT);
		this->addStatus(eStatus::MOVING_LEFT);

		break;
	}
	case DIK_RIGHT:
	{
		this->removeStatus(eStatus::LAYING_DOWN);
		this->removeStatus(eStatus::MOVING_LEFT);
		this->addStatus(eStatus::MOVING_RIGHT);

		break;
	}
	case DIK_DOWN:
	{
		this->addStatus(eStatus::LAYING_DOWN);
		break;
	}
	case DIK_UP:
	{
		this->addStatus(eStatus::LOOKING_UP);
		break;
	}
	case DIK_X:
	{
		this->addStatus(eStatus::SHOOTING);
		break;
	}
	default:
		break;
	}
}

void Bill::onKeyReleased(KeyEventArg * key_event)
{
	switch (key_event->_key)
	{
	case DIK_RIGHT:
	{
		this->removeStatus(eStatus::MOVING_RIGHT);
		break;
	}
	case DIK_LEFT:
	{
		this->removeStatus(eStatus::MOVING_LEFT);
		break;
	}
	case DIK_DOWN:
	{
		this->removeStatus(eStatus::LAYING_DOWN);
		break;
	}
	case DIK_UP:
	{
		this->removeStatus(eStatus::LOOKING_UP);
		break;
	}
	case DIK_X:
	{
		this->removeStatus(eStatus::SHOOTING);
		break;
	}
	default:
		break;
	}
}

void Bill::onCollisionBegin(CollisionEventArg * collision_event)
{
	if (collision_event->_otherObject->getId() == eID::BOX || collision_event->_otherObject->getId() == eID::BRIDGE)
	{
		//if (collision_event->_sideCollision == eDirection::TOP)
		{
			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::SHALLOWED);

			this->standing();
			//_canStand.push_back(true);
		}
	}
}

void Bill::onCollisionEnd(CollisionEventArg * collision_event)
{
	if (collision_event->_otherObject->getId() == eID::BOX || collision_event->_otherObject->getId() == eID::BRIDGE)
	{
		//if (_canStand.size() > 0)
			//_canStand.pop_front();

		//if(_canStand.size() == 0)
		{
			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING__DOWN);

			if (!this->isInStatus(eStatus::JUMPING))
				this->addStatus(eStatus::FALLING);
		}
	}
}

float Bill::checkCollision(BaseObject * object, float dt)
{
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	collisionBody->checkCollision(object, dt);

	return 0.0f;
}

void Bill::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));

	this->removeStatus(eStatus::JUMPING);
	this->removeStatus(eStatus::FALLING);
}

void Bill::moveLeft()
{
	this->setScaleX(-1);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed, move->getVelocity().y));
}

void Bill::moveRight()
{
	this->setScaleX(1);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Bill::jump()
{
	if ((this->getStatus() & eStatus::JUMPING) == eStatus::JUMPING)
		return;

	this->setStatus(eStatus(this->getStatus() | eStatus::JUMPING));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, BILL_JUMP_VEL));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);

}

void Bill::layDown()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, move->getVelocity().y));
}

void Bill::falling()
{

}

void Bill::addStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() | status));
}

void Bill::removeStatus(eStatus status)
{
	this->setStatus(eStatus(this->getStatus() & ~status));
}

bool Bill::isInStatus(eStatus status)
{
	return (this->getStatus() & status) == status;
}

GVector2 Bill::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bill::updateStatus(float dt)
{
	if ((this->getStatus() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT)
	{
		this->moveLeft();
	}
	else if ((this->getStatus() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)
	{
		this->moveRight();
	}
	else if ((this->getStatus() & eStatus::LAYING_DOWN) == eStatus::LAYING_DOWN)
	{
		this->layDown();
	}
	else if ((this->getStatus() & eStatus::FALLING) == eStatus::FALLING)
	{
		this->falling();
	}
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Bill::updateCurrentAnimateIndex()
{
	if (this->isInStatus(eStatus::SHOOTING) && (this->isInStatus(eStatus::LOOKING_UP) || this->isInStatus(eStatus::LAYING_DOWN)) && (this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT)))
	{
		_currentAnimateIndex = (eStatus)(this->getStatus() & ~(eStatus::SHOOTING));
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN) && this->isInStatus(eStatus::SHOOTING))
	{
		_currentAnimateIndex = eStatus::LAYING_DOWN;
	}
	else
	{
		_currentAnimateIndex = this->getStatus();
	}

	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::FALLING));
	}

	if ((_currentAnimateIndex & eStatus::HOLDING) == eStatus::HOLDING)
	{
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::HOLDING));
	}

	if ((_currentAnimateIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT || ((_currentAnimateIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT))
	{
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex | eStatus::RUNNING);
	}

	if ((_currentAnimateIndex & eStatus::JUMPING) == eStatus::JUMPING)
	{
		_currentAnimateIndex = eStatus::JUMPING;
	}
}
