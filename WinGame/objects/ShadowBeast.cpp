
#include "ShadowBeast.h"
#include "Stage3.h"

ShadowBeast::ShadowBeast(GVector2 position) : BaseEnemy(eID::SHADOW_BEAST)
{
	_position = position;
}

ShadowBeast::~ShadowBeast()
{
}

void ShadowBeast::init()
{
	_leftArm = new ShadowArm(_position + GVector2(-107, 52));
	_leftArm->init();
	_leftArm->setWise(false);

	_rightArm = new ShadowArm(_position + GVector2(107, 52));
	_rightArm->init();
	_rightArm->setWise(true);

	_mouth = new ShadowMouth(_position+ GVector2(0, 120));
	_mouth->init();
}
void ShadowBeast::update(float deltatime)
{
	if (_leftArm != nullptr)
	{
		_leftArm->update(deltatime);
	}
	if (_rightArm != nullptr)
	{
		_rightArm->update(deltatime);
	}
	if (_mouth != nullptr)
	{
		_mouth->update(deltatime);
	}
}
void ShadowBeast::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_leftArm != nullptr)
	{
		_leftArm->draw(spriteHandle, viewport);
	}
	if (_rightArm != nullptr)
	{
		_rightArm->draw(spriteHandle, viewport);
	}
	if (_mouth != nullptr)
	{
		_mouth->draw(spriteHandle, viewport);
	}
	
}
void ShadowBeast::release()
{
	if (_leftArm != nullptr)
	{
		_leftArm->release();
		delete _leftArm;
		_leftArm = nullptr;
	}
	if (_rightArm != nullptr)
	{
		_rightArm->release();
		delete _rightArm;
		_rightArm = nullptr;
	}
	if (_mouth != nullptr)
	{
		_mouth->release();
		delete _mouth;
		_mouth = nullptr;
	}
}
RECT ShadowBeast::getBounding()
{
	return RECT{ 0, 0, 0, 0 };
}

BaseObject* ShadowBeast::getLeftArm()
{
	return this->_leftArm;
}

BaseObject* ShadowBeast::getRigtArm()
{
	return this->_rightArm;
}

float ShadowBeast::checkCollision(BaseObject* object, float dt)
{
	return 0.0f;
}

/*  Shadow Arm */

void ShadowBeast::ShadowArm::init()
{
	srand(599999);
	Sprite* hand1 = SpriteManager::getInstance()->getSprite(eID::SHADOW_ARM);
	hand1->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_ARM, "arm"));
	hand1->setScale(SCALE_FACTOR);
	hand1->setPosition(_rootOfArm);

	Sprite* hand2 = SpriteManager::getInstance()->getSprite(eID::SHADOW_ARM);
	hand2->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_ARM, "arm"));
	hand2->setScale(SCALE_FACTOR);
	hand2->setPosition(_rootOfArm + GVector2(28, 14));

	Sprite* hand3 = SpriteManager::getInstance()->getSprite(eID::SHADOW_ARM);
	hand3->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_ARM, "arm"));
	hand3->setScale(SCALE_FACTOR);
	hand3->setPosition(_rootOfArm + GVector2(42, 42));

	Sprite* hand4 = SpriteManager::getInstance()->getSprite(eID::SHADOW_ARM);
	hand4->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_ARM, "arm"));
	hand4->setScale(SCALE_FACTOR);
	hand4->setPosition(_rootOfArm + GVector2(28, 70));

	_sprite = SpriteManager::getInstance()->getSprite(eID::SHADOW_ARM);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_ARM, "hand"));
	_sprite->setScale(SCALE_FACTOR);
	_sprite->setPosition(_rootOfArm + GVector2(0, 80));

	roundCircletime = 0.0f;
	CollisionBody* collisionBody = new CollisionBody(this);
	this->_componentList["CollisionBody"] = collisionBody;
	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin); //dư?

	_stopWatch = new StopWatch();
	_aimtime = new StopWatch();
	_trackBillTime = new StopWatch();
	
	_isClockWise = false;
	_isAiming = false;
	flagforTrackBill = false;
	counterforFire = 0;

	_handelem1 = HandElement{ hand1, 0.0f, 0 };
	_handelem2 = HandElement{ hand2, 0.0f, 32 };
	_handelem3 = HandElement{ hand3, 0.0f, 0 };
	_handelem4 = HandElement{ hand4, 0.0f, 0 };
	_arm	= HandElement{ _sprite, 0.0f, 0 };

	_flagAimFinish[0] = false;
	_flagAimFinish[1] = false;
	_flagAimFinish[2] = false;
	_flagAimFinish[3] = false;
	_waveCount = 0;
	_memStatus = normal;
}

void ShadowBeast::ShadowArm::initComponent(Sprite* sprite)
{
	auto roundmovement = new RoundMovement(0.0f, 0.0f, 0.0f, sprite);
	Movement* movement = new Movement(VECTOR2ZERO, VECTOR2ZERO, sprite);
	_componentList["Movement"] = movement;
	_componentList["RoundMovement"] = roundmovement;
}

ShadowBeast::ShadowArm::ShadowArm(GVector2 root) : BaseEnemy(eID::SHADOW_ARM)
{
	//for (int i = 0; i < 4; i++)

	_rootOfArm = root;
}

ShadowBeast::ShadowArm::~ShadowArm()
{
}

void ShadowBeast::ShadowArm::update(float deltatime)
{
	if (_memStatus == normal)
	{
		if (_stopWatch->isTimeLoop(1000))
		{
			int rd = rand() % 2;
			if (rd == 0)
			{
				changeWise();
			}
			_stopWatch->restart();
		}
		this->roundCircle(deltatime);
		if (_aimtime->isTimeLoop(5000))
		{
			_memStatus = aiming;
			_aimtime->restart();
			flagforTrackBill = false;
		}
	}
	if (_memStatus == aiming)
	{
		this->aimAttack(deltatime);
	}

	calculateLenght(_handelem3, _handelem2);
	calculateLenght(_handelem4, _handelem3);
	calculateLenght(_arm, _handelem4);

	this->updateElemPosition(_handelem1);
	this->updateElemPosition(_handelem2);
	this->updateElemPosition(_handelem3);
	this->updateElemPosition(_handelem4);
	this->updateElemPosition(_arm);

}

void  ShadowBeast::ShadowArm::changeWise()
{
	_isClockWise = !_isClockWise;
}
void ShadowBeast::ShadowArm::roundCircle(float deltatime)
{

	if (this->_isClockWise)
	{
		_handelem2._angle += 5.0 * deltatime / 1000;
		if (_handelem2._angle - _handelem3._angle > M_PI_4 / 2)
		{
			_handelem3._angle = _handelem2._angle - M_PI_4 / 2;		
		}
		if (_handelem3._angle - _handelem4._angle > M_PI_4 / 2)
		{
			_handelem4._angle = _handelem3._angle - M_PI_4 / 2;
		}
		if (_handelem4._angle - _arm._angle > M_PI_4 / 2)
		{
			_arm._angle = _handelem4._angle - M_PI_4 / 2;
		}
	}
	else
	{
		_handelem2._angle -= 5.0 * deltatime / 1000;

		if (_handelem2._angle - _handelem3._angle < -M_PI_4 / 2)
		{
			_handelem3._angle = _handelem2._angle + M_PI_4 / 2;
		}
		if (_handelem3._angle - _handelem4._angle < -M_PI_4 / 2)
		{
			_handelem4._angle = _handelem3._angle + M_PI_4 / 2;
		}
		if (_handelem4._angle - _arm._angle < -M_PI_4 / 2)
		{
			_arm._angle = _handelem4._angle + M_PI_4 / 2;
		}
	}
}
void ShadowBeast::ShadowArm::wave(float deltatime)
{
	if (this->_isClockWise)
	{
		_handelem2._angle += 2.8 * deltatime / 1000;
		if (_handelem2._angle - _handelem3._angle > M_PI_4 / 2)
		{
			_handelem3._angle = _handelem2._angle - M_PI_4 / 2;
		}
		if (_handelem3._angle - _handelem4._angle > M_PI_4 / 2)
		{
			_handelem4._angle = _handelem3._angle - M_PI_4 / 2;
		}
		if (_handelem4._angle - _arm._angle > M_PI_4 / 2)
		{
			_arm._angle = _handelem4._angle - M_PI_4 / 2;
			_isClockWise = !_isClockWise;
			_waveCount++;
		}
	}
	else
	{
		_handelem2._angle -= 2.8 * deltatime / 1000;

		if (_handelem2._angle - _handelem3._angle < -M_PI_4 / 2)
		{
			_handelem3._angle = _handelem2._angle + M_PI_4 / 2;
		}
		if (_handelem3._angle - _handelem4._angle < -M_PI_4 / 2)
		{
			_handelem4._angle = _handelem3._angle + M_PI_4 / 2;
		}
		if (_handelem4._angle - _arm._angle < -M_PI_4 / 2)
		{
			_arm._angle = _handelem4._angle + M_PI_4 / 2;
			_isClockWise = !_isClockWise;
			_waveCount++;
		}
	}
	if (_waveCount > 4)
	{
		_waveCount = 0;
		_memStatus = normal;
	}
}

void ShadowBeast::ShadowArm::aimAttack(float deltatime)
{
	if (this->_isClockWise)
	{
		float k = (_handelem2._angle + M_PI_2) / (2 * M_PI);
		if (abs(k - int(k)) >= 0.08)
		{
			if (flagforTrackBill == false)
			{
				roundCircle(deltatime);
				return;
			}
		}
		flagforTrackBill = true;

		_handelem3._angle += 5.0 * deltatime / 1000;
		_handelem4._angle += 5.0 * deltatime / 1000;
		_arm._angle += 5.0 * deltatime / 1000;

		if (_handelem3._angle - _handelem2._angle > 0.15)
		{
			_handelem3._angle = _handelem2._angle;
		}
		if (_handelem4._angle - _handelem3._angle > 0.15)
		{                     
			_handelem4._angle = _handelem3._angle;
		}
		if (_arm._angle - _handelem4._angle > 0.15)
		{
			_arm._angle = _handelem4._angle;
		}
	}
	else
	{
		float k = (_handelem2._angle + M_PI_2) / (2 * M_PI);
		if (abs(k - int(k)) >= 0.08)
		{
			if (flagforTrackBill == false)
			{
				roundCircle(deltatime);
				return;
			}
		}
		flagforTrackBill = true;

		//_handelem2._angle = -M_PI_2;
		_handelem3._angle -= 5.0 * deltatime / 1000;
		_handelem4._angle -= 5.0 * deltatime / 1000;
		_arm._angle -= 5.0 * deltatime / 1000;

		if (_handelem3._angle - _handelem2._angle <-0.15)
		{
			_handelem3._angle = _handelem2._angle ;
		}
		if (_handelem4._angle - _handelem3._angle < -0.15)
		{
			_handelem4._angle = _handelem3._angle ;
		}
		if (_arm._angle - _handelem4._angle < -0.15)
		{
			_arm._angle = _handelem4._angle;
		}
	}
	if (flagforTrackBill == true)
	{

		if (_trackBillTime->isStopWatch(700.0f))
		{
			auto billposition = ((Stage3*)SceneManager::getInstance()->getCurrentScene())->getBill()->getPosition();
			auto thisPosition = _rootOfArm;
			auto angle_tracked = atan((billposition.x - thisPosition.x) / (billposition.y - thisPosition.y));
			//if (_isClockWise)
			//	_handelem2._angle = M_PI_2 + angle_tracked;
			//else
				_handelem2._angle = -M_PI_2 - angle_tracked;
			
			_trackBillTime->restart();
			counterforFire++;
			if (counterforFire == 3)
			{
				counterforFire = 0;
				_memStatus = normal;
				shoot();
			}
		}
	}
}
void ShadowBeast::ShadowArm::setWise(bool isWise)
{
	this->_isClockWise = isWise;
}
void ShadowBeast::ShadowArm::shoot()
{
	auto billposition = ((Stage3*)SceneManager::getInstance()->getCurrentScene())->getBill()->getPosition();
	auto thisPosition = _arm._sprite->getPosition();
	auto angle_tracked = atan((billposition.x - thisPosition.x) / (thisPosition.y - billposition.y ));
	BulletManager::insertBullet(new BeastBullet(_arm._sprite->getPosition(), D3DXToDegree(- angle_tracked - M_PI)));
}
void ShadowBeast::ShadowArm::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	//_sprite->render(spriteHandle, viewport);
	_handelem1._sprite->render(spriteHandle, viewport);
	_handelem2._sprite->render(spriteHandle, viewport);
	_handelem3._sprite->render(spriteHandle, viewport);
	_handelem4._sprite->render(spriteHandle, viewport);
	_arm._sprite->render(spriteHandle, viewport);
}
void ShadowBeast::ShadowArm::release()
{
	SAFE_DELETE(_sprite);

}
RECT ShadowBeast::ShadowArm::getBounding()
{
	return BaseObject::getBounding();
}

float ShadowBeast::ShadowArm::checkCollision(BaseObject* object, float dt)
{
	return 0.0f;
}

void ShadowBeast::ShadowArm::calculateLenght(HandElement& currentElem, HandElement& prevElem)
{
	float delta_angle = currentElem._angle - prevElem._angle;
	if (delta_angle == 0)
	{
		currentElem._lenght = prevElem._lenght + 32;
	}
	else
	{
		float temp = M_PI - delta_angle - asin(prevElem._lenght * sin(delta_angle) / 32);
		currentElem._lenght = 32 * sin(temp) / sin(delta_angle);
	}
}

void ShadowBeast::ShadowArm::updateElemPosition(HandElement& elem)
{
	elem._sprite->setPosition(
		_rootOfArm + elem._lenght * GVector2(cos(elem._angle), sin(elem._angle))
		);
}


/* Shadow Mouth */


void ShadowBeast::ShadowMouth::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SHADOW_MOUTH);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_MOUTH, "mouth_opening1"));
	_sprite->setScale(SCALE_FACTOR);
	_sprite->setPosition(_startposition);

	_animations[eMouthStatus::OPENING] = new Animation(_sprite, 1);
	_animations[eMouthStatus::OPENING]->addFrameRect(eID::SHADOW_MOUTH, "mouth_opening1", "mouth_opening2", "mouth_opening3", "mouth_opening4", "mouth_opening5", "mouth_opening5", "mouth_opening4", "mouth_opening3", "mouth_opening2", "mouth_opening1", NULL);
	_animations[eMouthStatus::CLOSED] = new Animation(_sprite, 1);
	_animations[eMouthStatus::CLOSED]->addFrameRect(eID::SHADOW_MOUTH, "mouth_closed1", "mouth_closed2", "mouth_closed3", NULL);
	//_animations[eMouthStatus::CLOSING] = new Animation(_sprite, 1);
	//_animations[eMouthStatus::CLOSING]->addFrameRect(eID::SHADOW_MOUTH, "mouth_opening5", "mouth_opening4", "mouth_opening3", "mouth_opening2", "mouth_opening1", NULL);

	this->setMouthStatus(eMouthStatus::OPENING);
}
void ShadowBeast::ShadowMouth::update(float deltatime)
{
	Animation* current = _animations[this->getMouthStatus()];
	//_animations[this->getMouthStatus()]->update(deltatime);
	//if (current->getIndex() == 2)
	//{
	//	shoot();
	//}
	//if (this->getMouthStatus() == eMouthStatus::OPENING && current->getIndex() == 0)
	//	this->setMouthStatus(eMouthStatus::CLOSED);
	////else if (this->getMouthStatus() == eMouthStatus::CLOSING && current->getIndex() == 4)
	////	this->setMouthStatus(eMouthStatus::CLOSED);
	//else if (this->getMouthStatus() == eMouthStatus::CLOSED && current->getIndex() == 0)
	//	this->setMouthStatus(eMouthStatus::OPENING);
	_animations[this->getMouthStatus()]->update(deltatime);
	if (this->getMouthStatus() == eMouthStatus::CLOSED && current->getIndex() == 0)
		this->setMouthStatus(eMouthStatus::OPENING);
	//else if (this->getMouthStatus() == eMouthStatus::CLOSING && current->getIndex() == 4)
	//	this->setMouthStatus(eMouthStatus::CLOSED);
	else if (this->getMouthStatus() == eMouthStatus::OPENING && current->getIndex() == 0)
		this->setMouthStatus(eMouthStatus::CLOSED);
}
void ShadowBeast::ShadowMouth::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_animations[this->getMouthStatus()] != nullptr)
	{
		_animations[this->getMouthStatus()]->draw(spriteHandle, viewport);
	}
}
void ShadowBeast::ShadowMouth::release()
{
	SAFE_DELETE(_sprite);
	SAFE_DELETE(_animations[eMouthStatus::CLOSED]);
	SAFE_DELETE(_animations[eMouthStatus::OPENING]);
	SAFE_DELETE(_animations[eMouthStatus::CLOSING]);
}
RECT ShadowBeast::ShadowMouth::getBounding()
{
	return BaseObject::getBounding();
}
void ShadowBeast::ShadowMouth::shoot()
{
	//BulletManager::insertBullet(new BeastBullet(this->getPosition(), D3DXToDegree( M_PI + M_PI_2 / 3)));
	//BulletManager::insertBullet(new BeastBullet(this->getPosition(), D3DXToDegree( M_PI + 0)));
	//BulletManager::insertBullet(new BeastBullet(this->getPosition(), D3DXToDegree( M_PI - M_PI_2 / 3)));
}
ShadowBeast::ShadowMouth::ShadowMouth(GVector2 startposition) : BaseEnemy(eID::SHADOW_MOUTH)
{
	this->_startposition = startposition;
}

ShadowBeast::ShadowMouth::~ShadowMouth()
{
}

eMouthStatus ShadowBeast::ShadowMouth::getMouthStatus()
{
	return _mouthStatus;
}

void ShadowBeast::ShadowMouth::setMouthStatus(eMouthStatus status)
{
	_mouthStatus = status;
}
