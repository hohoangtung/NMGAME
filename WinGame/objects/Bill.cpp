#include "Bill.h"

Bill::Bill() : BaseObject(eID::BILL)
{
}

Bill::~Bill()
{
}

void Bill::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	_componentList["Movement"] = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Gravity"] = new Gravity(GVector2(0, -15), (Movement*)_componentList["Movement"]);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.25f);
	_animations[eStatus::NORMAL]->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->getId(), "normal_01"));
	_animations[eStatus::NORMAL]->addFrameRect(SpriteManager::getInstance()->getSourceRect(this->getId(), "normal_02"));

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

	_sprite->drawBounding(false);
	_sprite->setOrigin(GVector2(0.5f, 0.0f));
	
	this->setStatus(eStatus::NORMAL);

	//InputController::getInstance()->_keyPressed += (EventFunction)(&(onKeyPressed(this));

	
}

void Bill::update(float deltatime)
{
	_animations[this->getStatus()]->update(deltatime);

	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

	if (_sprite->getPositionY() < TEST_LAND && this->getStatus() != NORMAL)
	{
		this->setStatus(eStatus::NORMAL);
		this->standing();
		_sprite->setPositionY(TEST_LAND);
	}
}

void Bill::updateInput(float dt)
{
	if (InputController::getInstance()->isKeyDown(DIK_RIGHT))
	{
		this->moveRight();
	}
	else if (InputController::getInstance()->isKeyDown(DIK_LEFT))
	{
		this->moveLeft();
	}
	else if (InputController::getInstance()->isKeyDown(DIK_DOWN))
	{
		this->layDown();
	}
	else
	{
		this->standing();
	}

	if (InputController::getInstance()->isKeyPressed(DIK_SPACE))
	{
		this->jump();
	}

}

void Bill::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animations[this->getStatus()]->draw(spriteHandle, viewport);
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
	
}

void Bill::onKeyReleased(KeyEventArg * key_event)
{
}

void Bill::standing()
{
	if (this->getStatus() == JUMPING)
		return;

	this->setStatus(eStatus::NORMAL);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, 0));
	move->setAccelerate(GVector2(0, 0));

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::SHALLOWED);
}

void Bill::moveLeft()
{
	_sprite->setScaleX(-1);

	this->setStatus(eStatus::MOVING_LEFT);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-BILL_MOVE_SPEED, 0));
}

void Bill::moveRight()
{
	_sprite->setScaleX(1);

	this->setStatus(eStatus::MOVING_RIGHT);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(BILL_MOVE_SPEED, 0));
}

void Bill::jump()
{
	this->setStatus(eStatus::JUMPING);

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(move->getVelocity().x, BILL_JUMP_VEL));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
}

void Bill::layDown()
{
	this->setStatus(eStatus::LAYING_DOWN);
}

GVector2 Bill::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}
