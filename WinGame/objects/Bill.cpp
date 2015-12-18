#include "Bill.h"
#include "AirCraft.h"
#include "../debug.h"

Bill::Bill(int life) : BaseObject(eID::BILL)
{
	_canJumpDown = true;
	_lifeNum = life;
}

Bill::~Bill()
{
	
}

void Bill::init()
{
	__hook(&InputController::__eventkeyPressed, _input, &Bill::onKeyPressed);
	__hook(&InputController::__eventkeyReleased, _input, &Bill::onKeyReleased);

	_sprite = SpriteManager::getInstance()->getSprite(eID::BILL);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BILL, "normal_01"));

	auto movement = new Movement(GVector2(0, 0), GVector2(0, 0), _sprite);
	_componentList["Movement"] = movement;
	_componentList["Gravity"] = new Gravity(GVector2(0, -GRAVITY), movement);

	auto collisionBody = new CollisionBody(this);
	_componentList["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bill::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Bill::onCollisionEnd);

	_animations[eStatus::NORMAL] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL]->addFrameRect(eID::BILL, "normal_01", NULL);
	
	_animations[eStatus::NORMAL | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::NORMAL | eStatus::SHOOTING]->addFrameRect(eID::BILL, "normal_01", "normal_02", NULL);

	_animations[eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::RUNNING]->addFrameRect(eID::BILL, "run_01", "run_02", "run_03", "run_04", "run_05", "run_06", NULL);

	_animations[eStatus::JUMPING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::JUMPING]->addFrameRect(eID::BILL, "jump_01", "jump_02", "jump_03", NULL);

	_animations[eStatus::FALLING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::FALLING]->addFrameRect(eID::BILL, "run_03", NULL);

	_animations[eStatus::LAYING_DOWN] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LAYING_DOWN]->addFrameRect(eID::BILL, "lay_down_01", NULL);

	_animations[eStatus::LOOKING_UP] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP]->addFrameRect(eID::BILL, "shot_up_01", NULL);

	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::LOOKING_UP | eStatus::SHOOTING]->addFrameRect(eID::BILL, "shot_up_01", "shot_up_02" , NULL);

	_animations[eStatus::LOOKING_UP | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::LOOKING_UP | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_up_01", "run_shot_up_02", "run_shot_up_03", NULL);

	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::LAYING_DOWN | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_down_01", "run_shot_down_02", "run_shot_down_03", NULL);

	_animations[eStatus::SHOOTING | eStatus::RUNNING] = new Animation(_sprite, 0.12f);
	_animations[eStatus::SHOOTING | eStatus::RUNNING]->addFrameRect(eID::BILL, "run_shot_01", "run_shot_02", "run_shot_03", "run_shot_01", "run_shot_02", "run_shot_03", NULL);

	_animations[eStatus::SWIMING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SWIMING]->addFrameRect(eID::BILL, "swim_begin", "diving", "swimming", NULL);
	_animations[eStatus::SWIMING]->setLoop(false);

	_animations[eStatus::DIVING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::DIVING]->addFrameRect(eID::BILL, "diving", NULL);

	_animations[eStatus::SWIMING | RUNNING | SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SWIMING | RUNNING | SHOOTING]->addFrameRect(eID::BILL, "swimming_shot", NULL);

	_animations[eStatus::SWIMING | SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SWIMING | SHOOTING]->addFrameRect(eID::BILL, "swimming_shot", NULL);

	_animations[eStatus::SWIMING | RUNNING | LOOKING_UP | SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SWIMING | RUNNING | LOOKING_UP | SHOOTING]->addFrameRect(eID::BILL, "swimming_shotup", NULL);

	_animations[eStatus::SWIMING | LOOKING_UP | SHOOTING] = new Animation(_sprite, 0.1f);
	_animations[eStatus::SWIMING | LOOKING_UP | SHOOTING]->addFrameRect(eID::BILL, "swimming_shotup_stand", NULL);

	_animations[eStatus::DYING] = new Animation(_sprite, 0.2f);
	_animations[eStatus::DYING]->addFrameRect(eID::BILL, "dead_01", "dead_02", "dead_03", "dead_04", NULL);
	_animations[eStatus::DYING]->setLoop(false);

	this->setOrigin(GVector2(0.5f, 0.0f));
	this->setScale(SCALE_FACTOR);
	this->setStatus(eStatus::FALLING);
	this->setShootSpeed(SHOOT_SPEED);
	this->setMaxBullet(MAX_BULLET);

	_movingSpeed = BILL_MOVE_SPEED;
	_protectTime = PROTECT_TIME;

	// create stopWatch
	_stopWatch = new StopWatch();
	_shootStopWatch = new StopWatch();

	_currentGun = eBulletType::NORMAL_BULLET;

	// UI
	// tạo ở đây cho dễ cập nhật khi chết, khỏi trỏ lung tung
	_lifeUI = new LifeUI(GVector2(20, 30), this->getLifeNumber());
	_lifeUI->init();
}

void Bill::update(float deltatime)
{
	// đang protect thì đếm xuống
	if (_protectTime > 0)
	{
		_protectTime -= deltatime;
	}

	this->checkPosition();

	this->updateStatus(deltatime);

	this->updateCurrentAnimateIndex();

	_animations[_currentAnimateIndex]->update(deltatime);

	// Huỷ các bullet đã hết hiệu lực.
	this->deleteBullet();

	for (auto bullet : _listBullets)
	{
		bullet->update(deltatime);
	}

	// update component để sau cùng để sửa bên trên sau đó nó cập nhật đúng
	for (auto it = _componentList.begin(); it != _componentList.end(); it++)
	{
		it->second->update(deltatime);
	}

}


/*
*	author: Tung Ho
*	datemodified : 27/11/2015
*	kiểm tra và xoá đạn..
*	xoá các đạn ra khỏi màn hình và các đạn có status = destroy
*/
void Bill::deleteBullet()
{
	for (auto bullet : _listBullets)
	{
		if (bullet->getStatus() == eStatus::DESTROY)	// kiểm tra nếu là destroy thì loại khỏi list
		{
			bullet->release();
			// http://www.cplusplus.com/reference/algorithm/remove/
			auto rs1 = std::remove(_listBullets.begin(), _listBullets.end(), bullet);
			_listBullets.pop_back();			// sau khi remove thì còn một phần tử cuối cùng vôi ra. giống như dịch mảng. nên cần bỏ nó đi
			
			delete bullet;
			break;		// sau pop_back phần tử đi thì list bị thay đổi, nên vòng for-each không còn nguyên trạng nữa. -> break (mỗi frame chỉ remove được 1 đối tượng)
		}
	}
}
void Bill::updateInput(float dt)
{
	if (this->isInStatus(eStatus::DYING))
		return;
	
	if (_input->isKeyDown(DIK_Z))
	{
		if ((_currentGun & R_BULLET) == R_BULLET && _stopWatch->isStopWatch(_shootSpeed))
		{
			shoot();
			_stopWatch->restart();
		}
	}

	if (!_input->isKeyDown(DIK_Z) && _shootStopWatch->isStopWatch(200))
	{
		this->removeStatus(eStatus::SHOOTING);
		_shootStopWatch->restart();
	}
}

void Bill::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_protectTime > 0)
	{
		_animations[_currentAnimateIndex]->enableFlashes(true);
	}
	else
	{
		_animations[_currentAnimateIndex]->enableFlashes(false);
	}
		

	_animations[_currentAnimateIndex]->draw(spriteHandle, viewport);

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		(*it)->draw(spriteHandle, viewport);
	}

	// DRAW Life
	_lifeUI->draw(spriteHandle, viewport);
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
	SAFE_DELETE(_shootStopWatch);
	SAFE_DELETE(_reviveStopWatch);
	SAFE_DELETE(_lifeUI);
}

void Bill::onKeyPressed(KeyEventArg* key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;

	switch (key_event->_key)
	{
	case DIK_X:
	{
		if (!this->isInStatus(eStatus::LAYING_DOWN) || this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT))
		{
			if(!this->isInStatus(eStatus::SWIMING))
				this->jump();
		}
		else
		{
			if (_canJumpDown && !this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
			{
				this->removeStatus(eStatus::SHOOTING);
				this->addStatus(eStatus::JUMPING);
				this->addStatus(eStatus::FALLING);
			}
				
		}

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
		if (!this->isInStatus(eStatus::SWIMING))
		{
			this->removeStatus(eStatus::LOOKING_UP);
			this->addStatus(eStatus::LAYING_DOWN);
		}
		else
		{
			this->addStatus(eStatus::DIVING);
		}

		break;
	}
	case DIK_UP:
	{
		// không có TH nằm mà bắn lên
		if(!this->isInStatus(eStatus::LAYING_DOWN))
		this->addStatus(eStatus::LOOKING_UP);

		break;
	}
	case DIK_Z:
	{
		this->addStatus(eStatus::SHOOTING);

		if((_currentGun & eBulletType::R_BULLET) != eBulletType::R_BULLET)
		{
			shoot();
		}

		break;
	}
	default:
		break;
	}
}

void Bill::onKeyReleased(KeyEventArg * key_event)
{
	if (this->isInStatus(eStatus::DYING))
		return;

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
		this->removeStatus(eStatus::DIVING);
		break;
	}
	case DIK_UP:
	{
			this->removeStatus(eStatus::LOOKING_UP);
		break;
	}
	case DIK_Z:
	{
		_shootStopWatch->restart();

		break;
	}
	default:
		break;
	}
}

BaseObject* preObject;

void Bill::onCollisionBegin(CollisionEventArg * collision_arg)
{
	eID objectID = collision_arg->_otherObject->getId();
	switch (objectID)
	{
	case AIRCRAFT:
		break;
	case eID::LAND:
	case eID::BRIDGE:
	{

		break;
	}
	case eID::RIFLEMAN:
	case eID::SOLDIER:
	{

		break;
	}
	case eID::REDCANNON:
	case eID::WALL_TURRET:
	default:
		break;
	}
}

void Bill::onCollisionEnd(CollisionEventArg * collision_event)
{
	eID objectID = collision_event->_otherObject->getId();

	switch (objectID)
	{
	case AIRCRAFT:
		break;
	case eID::LAND:
	{
		if (preObject == collision_event->_otherObject)
		{
			// hết chạm với land là fall chứ ko có jump
			this->removeStatus(eStatus::JUMPING);
			preObject = nullptr;
		}
		break;
	}
	case eID::BRIDGE:
	break;
	default:
		break;
	}
}

float Bill::checkCollision(BaseObject * object, float dt)
{
	if (object->getStatus() == eStatus::DESTROY || this->isInStatus(eStatus::DYING))
		return 0.0f;

	auto collisionBody = (CollisionBody*)_componentList["CollisionBody"];
	eID objectId = object->getId();
	eDirection direction;

	if (objectId == eID::BRIDGE || objectId == eID::LAND || objectId == eID::ROCKFLY)
	{
		// nếu ko phải là nhảy xuống, mới dừng gravity
		if (!this->isInStatus(eStatus(eStatus::JUMPING | eStatus::FALLING)) && collisionBody->checkCollision(object, direction, dt, false))
		{
			// kt coi chổ đứng có cho nhảy xuống ko
			if (objectId == eID::LAND)
			{
				auto land = (Land*)object;
				_canJumpDown = land->canJump();

				eLandType preType = land->getType();

				// lấy type của preObject
				if (preObject != nullptr && preObject->getId() == eID::LAND)
				{
					preType = ((Land*)preObject)->getType();
				}
				
				// để trường hợp đang bơi mà lên bờ
				// bill cùng chạm water và grass
				// nó ưu tiên grass hơn không gán lại đang bơi
				if (land->getType() == eLandType::WATER)
				{
					// nếu trước đó không phải là nước thì mới cho bơi
					if (preType == eLandType::GRASS || preObject == nullptr || preObject->getId() == eID::BRIDGE)
					{
						this->addStatus(eStatus::SWIMING);

						if (this->isInStatus(eStatus::SHOOTING))
						{
							_animations[eStatus::SWIMING]->setIndex(2);
							_animations[eStatus::SWIMING]->canAnimate(false);
						}
					}
						
				}
				else if (this->isInStatus(eStatus::SWIMING))
				{
					if (direction == eDirection::TOP)
					{
						// lên bờ thì remove swim / nhảy lên
						this->removeStatus(eStatus::SWIMING);
						this->setPositionY(object->getBounding().top);

						// set lại animation swim
						_animations[eStatus::SWIMING]->restart();
					}
				}
			}

			if (objectId == eID::ROCKFLY)
			{
				_canJumpDown = false;
			}

			// nếu chạm top mà trừ trường hợp nhảy lên vận tốc rớt xuống nhỏ hơn 200
			if (direction == eDirection::TOP && !(this->getVelocity().y > -200 && this->isInStatus(eStatus::JUMPING)))
			{
				// vận tốc lớn hơn 200 hướng xuống => cho trường hợp nhảy từ dưới lên
				// xử lý đặc biệt, Collision body update position bt ko được
				// khi vào đây mới update position cho nó
				float moveX, moveY;
				if (collisionBody->isColliding(object, moveX, moveY, dt))
				{
					collisionBody->updateTargetPosition(object, direction, false, GVector2(moveX, moveY));
				}

				auto gravity = (Gravity*)this->_componentList["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->standing();
				
				preObject = object;
			}
		}
		else if(preObject == object)
		{
			// kiểm tra coi nhảy hết qua cái land cũ chưa
			// để gọi event end.
			collisionBody->checkCollision(object, dt, false);

			auto gravity = (Gravity*)this->_componentList["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING__DOWN);

			if(!this->isInStatus(eStatus::JUMPING) && !this->isInStatus(eStatus::FALLING))
			{
				this->addStatus(eStatus::FALLING);

				// 2 frame size khác nhau, đổi nó check va chạm dễ lộn, thêm offset để tránh TH đó
				if (this->getScale().x > 0)
					this->setPositionX(this->getPositionX() + 2);
				else
					this->setPositionX(this->getPositionX() - 2);
			}
		}
	}
	else
	{
		collisionBody->checkCollision(object, dt);
	}

	for (auto it = _listBullets.begin(); it != _listBullets.end(); it++)
	{
		if (object->getId() != eID::LAND && object->getId() != eID::BRIDGE)
		{
			if (objectId == eID::BOSS_STAGE1)
			{
				(*it)->checkCollision(((Boss*)object)->getGun1(), dt);
				(*it)->checkCollision(((Boss*)object)->getGun2(), dt);
				(*it)->checkCollision(((Boss*)object)->getShield(), dt);
				(*it)->checkCollision(((Boss*)object)->getRifleMan(), dt);
			}
			else
				(*it)->checkCollision(object, dt);
		}
	}

	return 0.0f;
}

void Bill::checkPosition()
{
	if (this->isInStatus(eStatus::DYING))
		return;

	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	if (this->getPositionY() < viewport->getBounding().bottom)
	{
		this->die();
		this->setStatus(eStatus::DYING);
	}
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
	// viewport
	auto viewport = SceneManager::getInstance()->getCurrentScene()->getViewport();
	GVector2 viewportPosition = viewport->getPositionWorld();
	float billPositionX = this->getPositionX();
	auto halfwidth = this->getSprite()->getFrameWidth() * this->getSprite()->getOrigin().x;
	// Không cho đi vượt cạnh trái
	if (billPositionX +  halfwidth - _movingSpeed * 0.33 <= viewportPosition.x) // hard code
	{
		this->setPositionX(viewportPosition.x + halfwidth);
		return;
	}
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
	if (this->isInStatus(eStatus::JUMPING) || this->isInStatus(eStatus::FALLING))
		return;

	this->addStatus(eStatus::JUMPING);

	//if (!this->isInStatus(eStatus::LAYING_DOWN))
	{
		auto move = (Movement*)this->_componentList["Movement"];
		move->setVelocity(GVector2(move->getVelocity().x, BILL_JUMP_VEL));
	}

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);

	SoundManager::getInstance()->Play(eSoundId::JUMP_SOUND);
}

void Bill::layDown()
{
	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(0, move->getVelocity().y));
}

void Bill::falling()
{
	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING__DOWN);
}

void Bill::shoot()
{
	if (this->isInStatus(eStatus::DIVING))
		return;

	float angle = 0.0f;
	auto direction = getAimingDirection();
	auto pos = this->getPosition() + GVector2(0, this->getSprite()->getFrameHeight() / 2);

	if (this->isInStatus(eStatus::LAYING_DOWN))
	{
		pos.y -= 5 * this->getScale().y;
	}
	
	if (direction == eDirection::TOP)
	{
		angle = TOP_SHOOT_ANGLE;
		pos.x += 5 * this->getScale().x;
		pos.y += this->getSprite()->getFrameHeight() / 3;
	}
	else if (direction == (eDirection::TOP | eDirection::RIGHT))
	{
		angle = TOPRIGHT_SHOOT_ANGLE;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 14 * this->getScale().y;
	}
	else if (direction == (eDirection::TOP | eDirection::LEFT))
	{
		angle = TOPLEFT_SHOOT_ANGLE;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 14 * this->getScale().y;
	}
	else if (direction == eDirection::LEFT)
	{
		angle = LEFT_SHOOT_ANGLE;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == eDirection::RIGHT)
	{
		angle = RIGHT_SHOOT_ANGLE;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == (eDirection::BOTTOM | eDirection::RIGHT))
	{
		angle = BOTRIGHT_SHOOT_ANGLE;
		pos.x += this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == (eDirection::BOTTOM | eDirection::LEFT))
	{
		angle = BOTLEFT_SHOOT_ANGLE;
		pos.x -= this->getSprite()->getFrameWidth() / 3;
		pos.y += 5 * this->getScale().y;
	}
	else if (direction == eDirection::BOTTOM)
	{
		angle = 180.0f;
	}

	if (this->isInStatus(eStatus::SWIMING))
		pos.y -= 8 * this->getScale().y;


	Bullet* bullet = this->getBulletFromGun(pos, angle);
	if (bullet == nullptr)
		return;

	//_listBullets.push_back(new Bullet(pos,(eBulletType)(BILL_BULLET | NORMAL_BULLET),angle));		// normalbullet -> hardcode
	_listBullets.push_back(bullet);		// normalbullet -> hardcode
	_listBullets.back()->init();
}

void Bill::revive()
{
	auto viewportPos = SceneManager::getInstance()->getCurrentScene()->getViewport()->getPositionWorld();
	this->setPosition(viewportPos.x, WINDOW_HEIGHT);
	
	// reset value
	this->setScaleX(SCALE_FACTOR);
	this->setStatus(eStatus::JUMPING);
	this->setLifeNumber(_lifeNum - 1);

	preObject = nullptr;

	_currentGun = eBulletType::NORMAL_BULLET;
	_protectTime = PROTECT_TIME;

	_animations[eStatus::SWIMING]->restart();

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(100, 0));

	auto gravity = (Gravity*)this->_componentList["Gravity"];
	gravity->setStatus(eGravityStatus::FALLING__DOWN);

	// cập nhật UI
	_lifeUI->setLifeNumber(_lifeNum);
}

void Bill::changeBulletType(eAirCraftType type)
{
	SoundManager::getInstance()->Play(eSoundId::EAT_ITEM);
	switch (type)
	{
	case L:
		this->_currentGun = eBulletType::L_BULLET;
		this->setMaxBullet(-1);
		break;
	case B:
		this->_currentGun = eBulletType::NORMAL_BULLET;
		this->setMaxBullet(4);
		break;
	case F:
		this->_currentGun = eBulletType::F_BULLET;
		this->setMaxBullet(2);
		break;
	case S:
		this->_currentGun = eBulletType::S_BULLET; 
		this->setMaxBullet(2);
		break;
	case M:
		this->_currentGun = eBulletType::M_BULLET;
		this->setMaxBullet(6);
		break;
	case R:
		if (this->_currentGun != eBulletType::L_BULLET)
		{
			this->_currentGun = eBulletType(this->_currentGun | eBulletType::R_BULLET);
			this->setMaxBullet(-1);
		}
			
		break;
	default:
		break;
	}
}

// Từ thuộc tính currentGun mà chọn loại đoạn trả về thích hợp
Bullet* Bill::getBulletFromGun(GVector2 position, float angle)
{
	Bullet* bullet = nullptr;
	if ((_currentGun & NORMAL_BULLET) == NORMAL_BULLET)
	{
		if (_listBullets.size() >= _maxBullet && (_currentGun & R_BULLET ) != R_BULLET)
			return nullptr;

		bullet = new Bullet(position, (eBulletType)(BILL_BULLET | NORMAL_BULLET), angle);
		SoundManager::getInstance()->Play(eSoundId::BASE_BULLET_FIRE);
	}
	else if ((_currentGun & L_BULLET) == L_BULLET)
	{
		if (this->_listBullets.empty() == false)
		{
			_listBullets.clear();
		}
		bullet = new LGun(position, angle);
		SoundManager::getInstance()->Play(eSoundId::LBULLET_FIRE);
	}
	else if ((_currentGun & F_BULLET) == F_BULLET)
	{
		if (_listBullets.size() >= _maxBullet && (_currentGun & R_BULLET) != R_BULLET)
			return nullptr; 
		bullet = new FBullet(position, angle);
		SoundManager::getInstance()->Play(eSoundId::FBULLET_FIRE);
	}
	else if ((_currentGun & S_BULLET) == S_BULLET)
	{
		if (_listBullets.size() >= _maxBullet && (_currentGun & R_BULLET) != R_BULLET)
			return nullptr;
		bullet = new SBullet(position, angle);
		SoundManager::getInstance()->Play(eSoundId::SBULLET_FIRE);
	}
	else if ((_currentGun & M_BULLET) == M_BULLET)
	{
		if (_listBullets.size() >= _maxBullet && (_currentGun & R_BULLET) != R_BULLET)
			return nullptr;
		bullet = new MBullet(position, angle);
		SoundManager::getInstance()->Play(eSoundId::MBULLET_FIRE);
	}
	return bullet;
}

void Bill::die()
{
	if (_protectTime > 0)
		return;

	auto move = (Movement*)this->_componentList["Movement"];
	move->setVelocity(GVector2(-BILL_MOVE_SPEED * (this->getScale().x / SCALE_FACTOR), BILL_JUMP_VEL));

	auto g = (Gravity*)this->_componentList["Gravity"];
	g->setStatus(eGravityStatus::FALLING__DOWN);
	SoundManager::getInstance()->Play(eSoundId::DEAD);
}

void Bill::setLifeNumber(int number)
{
	if (number != _lifeNum)
		_lifeNum = number;
}

int Bill::getLifeNumber()
{
	return _lifeNum;
}

void Bill::setShootSpeed(float speed)
{
	if (speed != _shootSpeed)
		_shootSpeed = speed;
}

float Bill::getShootSpeed()
{
	return _shootSpeed;
}

void Bill::setMaxBullet(int number)
{
	if (number != _maxBullet)
		_maxBullet = number;
}

int Bill::getMaxBullet()
{
	return _maxBullet;
}

void Bill::setStatus(eStatus status)
{
	// lặn / đang được bảo vệ  ko có chết
	if ((status & eStatus::DYING) == eStatus::DYING && (this->isInStatus(eStatus::DIVING) || _protectTime > 0))
		return;

	_status = status;
}

GVector2 Bill::getVelocity()
{
	auto move = (Movement*)this->_componentList["Movement"];
	return move->getVelocity();
}

void Bill::updateStatus(float dt)
{
	if (this->isInStatus(eStatus::DYING))
	{
		if (_lifeNum <= 0)
		{
			// thua cnmr
			return;
		}

		// còn mạng thì hồi sinh
		if (_reviveStopWatch == nullptr)
			_reviveStopWatch = new StopWatch();

		if (!_animations[eStatus::DYING]->isAnimate() && _reviveStopWatch->isStopWatch(REVIVE_TIME))
		{
			this->revive();
			SAFE_DELETE(_reviveStopWatch);
		}

		return;
	}

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
	// _currentAnimateIndex là key của animate thôi nên có một số bị dư, nếu dùng chung status
	// vậy cần convert lại tương ứng với key trong animate

	if (this->isInStatus(eStatus::SHOOTING) && !this->isInStatus(eStatus::SWIMING) && (this->isInStatus(eStatus::LOOKING_UP) || this->isInStatus(eStatus::LAYING_DOWN)) && (this->isInStatus(eStatus::MOVING_LEFT) || this->isInStatus(eStatus::MOVING_RIGHT)))
	{
		// đang di chuyển mà shoot
		// animate ko có shooting nên bỏ nó ra
		_currentAnimateIndex = (eStatus)(this->getStatus() & ~(eStatus::SHOOTING));
	}
	else if (this->isInStatus(eStatus::LAYING_DOWN) && this->isInStatus(eStatus::SHOOTING))
	{
		// đang nằm và bắn
		// và ko có đang nhảy, thì animate NẰM thôi
		if (!this->isInStatus(eStatus::JUMPING))
			_currentAnimateIndex = eStatus::LAYING_DOWN;
		else
			_currentAnimateIndex = eStatus::JUMPING;
	}
	else
	{
			// trường hợp còn lại gán luôn
			_currentAnimateIndex = this->getStatus();
		}

	if ((_currentAnimateIndex & eStatus::FALLING) == eStatus::FALLING)
	{
		_currentAnimateIndex = eStatus::FALLING;

	}

	if ((_currentAnimateIndex & eStatus::HOLDING) == eStatus::HOLDING)
	{
		// không có animate HOLDING nên bỏ nó ra
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::HOLDING));
	}

	if ((_currentAnimateIndex & eStatus::MOVING_LEFT) == eStatus::MOVING_LEFT || ((_currentAnimateIndex & eStatus::MOVING_RIGHT) == eStatus::MOVING_RIGHT))
	{
		// animate move left/right xài cung 1 animate là running nên cũng bỏ nó ra
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex & ~(eStatus::MOVING_LEFT | MOVING_RIGHT));
		_currentAnimateIndex = (eStatus)(_currentAnimateIndex | eStatus::RUNNING);
	}

	if ((_currentAnimateIndex & eStatus::JUMPING) == eStatus::JUMPING)
	{
		// nếu jump thì chỉ vẽ jump thôi
		_currentAnimateIndex = eStatus::JUMPING;
	}

	// đang bơi
	if ((_currentAnimateIndex & eStatus::SWIMING) == eStatus::SWIMING)
	{
		if ((_currentAnimateIndex & eStatus::SHOOTING) != eStatus::SHOOTING)
		{
			_currentAnimateIndex = eStatus::SWIMING;
		}

		if (this->isInStatus(eStatus::DIVING))
		{
			_currentAnimateIndex = eStatus::DIVING;
		}
	}

	// chết
	if (this->isInStatus(eStatus::DYING))
	{
		_currentAnimateIndex = eStatus::DYING;
	}
}

eDirection Bill::getAimingDirection()
{
	eDirection direction;

	if (this->getScale().x < 0)
		direction = eDirection::LEFT;
	else
		direction = eDirection::RIGHT;

	//if (this->isInStatus(eStatus::JUMPING))
	//	return direction;

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
		else if(this->isInStatus(eStatus::JUMPING))
			direction = eDirection::BOTTOM;
	}

	return direction;
}

void Bill::forceMoveRight()
{
	onKeyPressed(new KeyEventArg(DIK_RIGHT));
}
void Bill::unforceMoveRight()
{
	onKeyReleased(new KeyEventArg(DIK_RIGHT));
}
void Bill::forceMoveLeft()
{
	onKeyPressed(new KeyEventArg(DIK_LEFT));
}
void Bill::unforceMoveLeft()
{
	onKeyReleased(new KeyEventArg(DIK_LEFT));
}
void Bill::forceJump()
{
	onKeyPressed(new KeyEventArg(DIK_X));
}
void Bill::unforceJump()
{
	onKeyReleased(new KeyEventArg(DIK_X));
}
void Bill::removeGravity()
{
	auto graivity = (Gravity*)(this->_componentList.find("Gravity")->second);
	graivity->setGravity(VECTOR2ZERO);
}


void Bill::unhookinputevent()
{
	__unhook(_input);
}
