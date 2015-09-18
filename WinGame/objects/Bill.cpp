#include "Bill.h"

Bill::Bill() : BaseObject(eID::BILL)
{
}

Bill::~Bill()
{
}

void Bill::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Bill::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Bill::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	_componentList["Movement"] = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Gravity"] = new Gravity(GVector2(0, - GRAVITY), (Movement*)_componentList["Movement"]);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.25f);
	_animations[eStatus::NORMAL]->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->getId(), "normal_01"));
	
	_animations[eStatus::NORMAL | eStatus::SHOOTING] = new Animation(_sprite, 0.25f);
	_animations[eStatus::NORMAL | eStatus::SHOOTING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->getId(), "normal_01"));
	_animations[eStatus::NORMAL | eStatus::SHOOTING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->getId(), "normal_02"));

	_animations[eStatus::MOVING_RIGHT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_01"));
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_02"));
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_03"));
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_04"));
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_05"));
	_animations[eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_06"));

	_animations[eStatus::MOVING_LEFT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_01"));
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_02"));
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_03"));
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_04"));
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_05"));
	_animations[eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_06"));

	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "jump_01"));
	_animations[eStatus::JUMPING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "jump_02"));
	_animations[eStatus::JUMPING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "jump_03"));

	_animations[eStatus::LAYING_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "lay_down_01"));

	_animations[eStatus::LOOKING_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "shot_up_01"));

	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "shot_up_01"));
	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "shot_up_02"));


	_animations[eStatus::LOOKING_UP | eStatus::MOVING_RIGHT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_01"));
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_02"));
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_03"));

	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_RIGHT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_01"));
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_02"));
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_03"));


	_animations[eStatus::LOOKING_UP | eStatus::MOVING_LEFT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_01"));
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_02"));
	_animations[eStatus::LOOKING_UP | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_up_03"));

	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_LEFT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_01"));
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_02"));
	_animations[eStatus::LAYING_DOWN | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_down_03"));

	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_01"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_02"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_03"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_01"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_02"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_LEFT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_03"));

	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_01"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_02"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_03"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_01"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_02"));
	_animations[eStatus::SHOOTING | eStatus::MOVING_RIGHT]->addFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "run_shot_03"));

	_sprite->drawBounding(false);
	_sprite->setOrigin(GVector2(0.5f, 0.0f));

	this->setStatus(eStatus::NORMAL);
	this->setState(eStatus::NORMAL);
}

void Bill::update(float deltatime)
{
	if (_sprite->getPositionY() < TEST_LAND)
	{
		_sprite->setPositionY(TEST_LAND);

		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);

		this->setState(this->getState() & ~eStatus::JUMPING);
		this->standing();
	}

	this->updateStatus(deltatime);

	this->updateCurrentState();

	_animations[_currentAnimateState]->update(deltatime);

	// update component ?? sau cùng ?? s?a bên trên sau ?ó nó c?p nh?t ?úng
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
	_animations[_currentAnimateState]->draw(spriteHandle, viewport);
}

void Bill::release()
{
	_sprite->release();
	_animations.clear();
}

void Bill::setPosition(float x, float y)
{
	_sprite->setPosition(x, y);
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
		this->removeState(eStatus::LAYING_DOWN);
		this->removeState(eStatus::MOVING_RIGHT);
		this->setState(this->getState() | eStatus::MOVING_LEFT);

		break;
	}
	case DIK_RIGHT:
	{
		this->removeState(eStatus::LAYING_DOWN);
		this->removeState(eStatus::MOVING_LEFT);
		this->setState(this->getState() | eStatus::MOVING_RIGHT);

		break;
	}
	case DIK_DOWN:
	{
		this->setState(this->getState() | eStatus::LAYING_DOWN);
		break;
	}
	case DIK_UP:
	{
		this->setState(this->getState() | eStatus::LOOKING_UP);
		break;
	}
	case DIK_X:
	{
		this->addState(eStatus::SHOOTING);
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
		this->setState(this->getState() & ~eStatus::MOVING_RIGHT);
		break;
	}
	case DIK_LEFT:
	{
		this->setState(this->getState() & ~eStatus::MOVING_LEFT);
		break;
	}
	case DIK_DOWN:
	{
		this->removeState(eStatus::LAYING_DOWN);
		break;
	}
	case DIK_UP:
	{
		this->removeState(eStatus::LOOKING_UP);
		break;
	}
	case DIK_X:
	{
		this->removeState(eStatus::SHOOTING);
		break;
	}
	default:
		break;
	}
}

void Bill::standing()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
}

void Bill::moveLeft()
{
	_sprite->setScaleX(-1);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-BILL_MOVE_SPEED, move->getVelocity().y));
}

void Bill::moveRight()
{
	_sprite->setScaleX(1);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(BILL_MOVE_SPEED, move->getVelocity().y));
}

void Bill::jump()
{
	if ((this->getState() & eStatus::JUMPING) == eStatus::JUMPING)
		return;

	this->setState(this->getState() | eStatus::JUMPING);

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

void Bill::setState(int state)
{
	if (_state != state)
		_state = state;
}

void Bill::addState(int state)
{
	this->setState(this->getState() | state);
}

void Bill::removeState(int state)
{
	this->setState(this->getState() & ~state);
}

bool Bill::isInState(int state)
{
	return (this->getState() & state) == state;
}

int Bill::getState()
{
	return _state;
}

GVector2 Bill::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bill::updateStatus(float dt)
{
	if ((this->getState() & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT)
	{
		this->moveLeft();
	}
	else if ((this->getState() & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT)
	{
		this->moveRight();
	}
	else if ((this->getState() & eStatus::LAYING_DOWN) == eStatus::LAYING_DOWN)
	{
		this->layDown();
	}
	else if ((this->getState() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Bill::updateCurrentState()
{
	if (this->isInState(eStatus::JUMPING))
	{
		_currentAnimateState = eStatus::JUMPING;
	}
	else if (this->isInState(eStatus::SHOOTING) && (this->isInState(eStatus::LOOKING_UP) || this->isInState(eStatus::LAYING_DOWN))&& (this->isInState(eStatus::MOVING_LEFT) || this->isInState(eStatus::MOVING_RIGHT)))
	{
		_currentAnimateState = _state & ~eStatus::SHOOTING;
	}
	else if (this->isInState(eStatus::LAYING_DOWN) && this->isInState(eStatus::SHOOTING))
	{
		_currentAnimateState = eStatus::LAYING_DOWN;
	}
	else
	{
		_currentAnimateState = _state;
	}
}
