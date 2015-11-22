#include "Bill.h"
#include "AirCraft.h"

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
	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);
	//_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), _sprite);	//test 7ung // vinh: cái này nó làm cho sprite ko cập nhật đc velocity, kt va chạm ko đc

	auto collisionBody = new CollisionBody(this);
	//collisionBody->setPhysicsObjects(ePhysicsBody::LAND);		// set kiểu mà nó va chạm vật lý dừng lại, mặc định ko dừng lại nhưng vẫn báo begin/end

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
	this->setScale(SCALE_FACTOR);

	_sideCollide = false;
	_movingSpeed = BILL_MOVE_SPEED;

	this->setStatus(eStatus::FALLING);

	// create stopWatch
	_stopWatch = new StopWatch();
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

	// bullet
	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->update(deltatime);

		// tạm để cho nó hết màn hình nó xóa
		//if ((*it)->getPositionX() < 0 || (*it)->getPositionX() > SceneManager::getInstance()->getCurrentScene()->getViewport()->getWidth() ||
		//	(*it)->getPositionY() < 0 || (*it)->getPositionY() > SceneManager::getInstance()->getCurrentScene()->getViewport()->getHeight()
		//	)
		//{
		//	auto temp = it;
		//	it++;
		//	_listBullets.erase(temp);
		//}

		//if (_listBullets.size() <= 0)
		//	break;
	}

	// update component để sau cùng để sửa bên trên sau đó nó cập nhật đúng
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

}

void Bill::updateInput(float dt)
{
	if (_input->isKeyDown(DIK_X))
	{
		if (_stopWatch->isStopWatch(SHOOT_SPEED))
		{
			shoot();
			_stopWatch->restart();
		}
	}
}

void Bill::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	_animations[_currentAnimateIndex]->draw(spriteHandle, viewport);

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->draw(spriteHandle, viewport);
	}
}

void Bill::release()
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

	SAFE_DELETE(_stopWatch);
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

		// shoot
		//this->shoot();

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

void Bill::onCollisionBegin(CollisionEventArg * collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();
	switch (objectID)
	{
	case AIRCRAFT:
		break;
	}
}

void Bill::onCollisionEnd(CollisionEventArg * collision_event)
{

}

BaseObject* preObject;

float Bill::checkCollision(BaseObject * object, float dt)
{
	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::BOX || objectId == eID::BRIDGE || objectId == eID::LAND)
	{
		// nếu ko phải là nhảy xuống, mới dừng gravity
		if (!this->isInStatus(eStatus(eStatus::JUMPING | eStatus::LAYING_DOWN)) && collisionBody->checkCollision(object, direction, dt))
		{
			if (direction == eDirection::TOP && this->getVelocity().y < 0)
			{
				auto gravity = (Gravity*)this->_componentList["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->standing();

				preObject = object;
			}
		}
		else if(preObject == object)
		{
			preObject = nullptr;

			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING__DOWN);

			if (!this->isInStatus(eStatus::JUMPING))
				this->addStatus(eStatus::FALLING);
		}
	}
	else if (objectId == eID::AIRCRAFT)
	{
		if (collisionBody->checkCollision(object, direction, dt))
		{
			auto aircraft = ((AirCraft*) object);
			auto billstatus = this->getStatus();
			if (((billstatus | eStatus::SHOOTING) == billstatus) && aircraft->getStatus() == eStatus::NORMAL)
			{
				// Trường hợp máy bay còn nguyên, vừa bắn vừa đứng đó ăn luôn.
				aircraft->setStatus(eStatus::BURST);
				aircraft->setExplored();
				this->changeBulletType(aircraft->getType());
			}
			else
			{
				// Trường hợp bắn máy bay xong chạy lại ăn.
				if (aircraft->getStatus() == eStatus::EXPLORED)
				{
					aircraft->setStatus(eStatus::DESTROY);
					this->changeBulletType(aircraft->getType());
				}
			}
		}
	}
	else
	{
		collisionBody->checkCollision(object, dt);
	}


	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->checkCollision(object, dt);
	}

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
	if(this->getScale().x > 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-_movingSpeed, move->getVelocity().y));
}

void Bill::moveRight()
{
	if (this->getScale().x < 0)
		this->setScaleX(this->getScale().x * (-1));

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(_movingSpeed, move->getVelocity().y));
}

void Bill::jump()
{
	if ((this->getStatus() & eStatus::JUMPING) == eStatus::JUMPING)
		return;

	this->addStatus(eStatus::JUMPING);

	if (!this->isInStatus(eStatus::LAYING_DOWN))
	{
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(move->getVelocity().x, BILL_JUMP_VEL));
	}

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

void Bill::shoot()
{
	float angle = 0.0f;
	auto direction = getAimingDirection();
	auto pos = this->getPosition() + GVector2(0, this->getSprite()->getFrameHeight() / 2);

	if (this->isInStatus(eStatus::LAYING_DOWN))
	{
		pos.y -= 5 * this->getScale().y;
	}
	
	if (direction == eDirection::TOP)
	{
		angle = 0.0f;
		pos.x += 5 * this->getScale().x;
		pos.y += this->getSprite()->getFrameHeight() / 3;
	}
	else if (direction == (eDirection::TOP | eDirection::RIGHT))
	{
		angle = 60.0f;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 14 * this->getScale().y;
	}
	else if (direction == (eDirection::TOP | eDirection::LEFT))
	{
		angle = -60.0f;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 14 * this->getScale().y;
	}
	else if (direction == eDirection::LEFT)
	{
		angle = -90.0f;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == eDirection::RIGHT)
	{
		angle = 90.0f;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == (eDirection::BOTTOM | eDirection::RIGHT))
	{
		angle = 120.0f;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == (eDirection::BOTTOM | eDirection::LEFT))
	{
		angle = -120.0f;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
		
	_listBullets.push_back(new Bullet(pos, angle));
	_listBullets.back()->init();
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
		if ((_currentAnimateIndex & eStatus::LAYING_DOWN) == eStatus::LAYING_DOWN)
		{
			_currentAnimateIndex = eStatus::NORMAL;
		}
		else
		{
			_currentAnimateIndex = eStatus::JUMPING;
		}
	}
}

eDirection Bill::getAimingDirection()
{
	eDirection direction;

	if (this->getScale().x < 0)
		direction = eDirection::LEFT;
	else
		direction = eDirection::RIGHT;

	if (this->isInStatus(eStatus::LOOKING_UP))
	{
		if (_input->getInstance()->isKeyDown(DIK_LEFT) || _input->getInstance()->isKeyDown(DIK_RIGHT))
			direction = (eDirection)(direction | eDirection::TOP);
		else
			direction = eDirection::TOP;
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN))
	{
		if (_input->getInstance()->isKeyDown(DIK_LEFT) || _input->getInstance()->isKeyDown(DIK_RIGHT))
			direction = (eDirection)(direction | eDirection::BOTTOM);
	}

	return direction;
}

void Bill::changeBulletType(eAirCraftType type)
{
	// do somehthing.
	// Khi va chạm với cái máy bay lúc bị bắn xong thì bên aircraft gọi hàm này.
	// Khi nào có nhiều kiểu đạn thì ông Vinh thêm vào hàm này nhé.
	OutputDebugString(L"Ăn đạn, thay đạn");
}