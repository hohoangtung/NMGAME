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
}

void Bill::update(float deltatime)
{
	if (_sprite->getPositionY() < TEST_LAND)
	{
		_sprite->setPositionY(TEST_LAND);

		auto gravity = (Gravity*)this->_componentList["Gravity"];
		gravity->setStatus(eGravityStatus::SHALLOWED);

		this->removeStatus(eStatus::JUMPING);
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
	else if ((this->getStatus() & eStatus::JUMPING) != eStatus::JUMPING)
	{
		this->standing();
	}
}

void Bill::updateCurrentAnimateIndex()
{
	if (this->isInStatus(eStatus::JUMPING))
	{
		_currentAnimateIndex = eStatus::JUMPING;
	}
	else if (this->isInStatus(eStatus::SHOOTING) && (this->isInStatus(eStatus::LOOKING_UP) || this->isInStatus(eStatus::LAYING_DOWN))&& (this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT)))
	{
		_currentAnimateIndex = (eStatus)(this->getStatus() & ~eStatus::SHOOTING);
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN) && this->isInStatus(eStatus::SHOOTING))
	{
		_currentAnimateIndex = eStatus::LAYING_DOWN;
	}
	else
	{
		_currentAnimateIndex = this->getStatus();
	}
}
