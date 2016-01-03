
#include "ShadowBeast.h"
#include "../Scenes/Stage3.h"

ShadowBeast::ShadowBeast(GVector2 position) : BaseEnemy(eID::SHADOW_BEAST)
{
	_position = position;
}

ShadowBeast::~ShadowBeast()
{
}

void ShadowBeast::init()
{
	_leftArm = new ShadowArm(_position + GVector2(-115, 60));
	_leftArm->init();
	_leftArm->setWise(false);

	_rightArm = new ShadowArm(_position + GVector2(115, 60));
	_rightArm->init();
	_rightArm->setWise(true);

	_mouth = new ShadowMouth(_position+ GVector2(0, 120));
	_mouth->init();
	_flagPlayedDestroyBoss = false;
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
		if (_mouth->getStatus() == eStatus::BURST)
		{
			if (_leftArm->getStatus() == eStatus::NORMAL)
			{
				_leftArm->setStatus(eStatus::BURST);
			}			
			if (_rightArm->getStatus() == eStatus::NORMAL)
			{
				_rightArm->setStatus(eStatus::BURST);
			}
		}
	}
	if (_flagPlayedDestroyBoss == false)
	{
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BOSS) == true)
		{
			_flagPlayedDestroyBoss = true;
		}
	}
	else
	{
		if (SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BOSS) == false)
		{
			this->setStatus(eStatus::DYING);
			//SoundManager::getInstance()->Play(eSoundId::WINGAME);
			//_flagPlayedDestroyBoss = false;
		}
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
	return RECT();
}

BaseObject* ShadowBeast::getLeftArm()
{
	return this->_leftArm;
}

BaseObject* ShadowBeast::getRigtArm()
{
	return this->_rightArm;
}
BaseObject* ShadowBeast::getMouth()
{
	return _mouth;
}
float ShadowBeast::checkCollision(BaseObject* object, float dt)
{
	return 0.0f;
}

/*  Shadow Arm */

void ShadowBeast::ShadowArm::init()
{
	srand(599999);
	_hitpoint = 16;
	_score = 2000;
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
	_explosion = nullptr;

	_isClockWise = false;
	_isAiming = false;
	flagforTrackBill = false;
	counterforFire = 0;

	_handelem1 = HandElement( hand1, 0.0f, 0 );
	_handelem2 = HandElement( hand2, 0.0f, 32 );
	_handelem3 = HandElement( hand3, 0.0f, 0 );
	_handelem4 = HandElement( hand4, 0.0f, 0 );
	_arm = HandElement( _sprite, 0.0f, 0 );

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
	if (this->getStatus() == eStatus::NORMAL)
	{
		updateStatusNormal(deltatime);
		if (this->getHitpoint() <= 0)
		{
			this->setStatus(eStatus::BURST);
			SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
		}
	}
	else if (this->getStatus() == eStatus::BURST)
		updateStatusBurst(deltatime);


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
	if (this->getStatus() == eStatus::NORMAL)
	{
		_handelem1._sprite->render(spriteHandle, viewport);
		_handelem2._sprite->render(spriteHandle, viewport);
		_handelem3._sprite->render(spriteHandle, viewport);
		_handelem4._sprite->render(spriteHandle, viewport);
		_arm._sprite->render(spriteHandle, viewport);
	}
	else if (this->getStatus() == eStatus::BURST)
	{
		
		if (_explosion != nullptr)
		{
			_explosion->setPosition(_handelem1._sprite->getPosition());
			_explosion->draw(spriteHandle, viewport);

			_explosion->setPosition(_handelem2._sprite->getPosition());
			_explosion->draw(spriteHandle, viewport);

			_explosion->setPosition(_handelem3._sprite->getPosition());
			_explosion->draw(spriteHandle, viewport);

			_explosion->setPosition(_handelem4._sprite->getPosition());
			_explosion->draw(spriteHandle, viewport);

			_explosion->setPosition(_arm._sprite->getPosition());
			_explosion->draw(spriteHandle, viewport);
		}
	}
}
void ShadowBeast::ShadowArm::release()
{
	SAFE_DELETE(_sprite);

}
RECT ShadowBeast::ShadowArm::getBounding()
{
	if (this->getStatus() != eStatus::NORMAL)
		return RECT();
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


void ShadowBeast::ShadowArm::updateStatusBurst(float deltatime)
{
	if (_explosion == nullptr )
	{
		_explosion = new  Explosion(2);
		_explosion->init();
		_explosion->setScale(SCALE_FACTOR);
	}
	else
	{
		_explosion->update(deltatime);
		if (_explosion->getStatus() == eStatus::DESTROY)
		{
			this->setStatus(eStatus::DESTROY);
		}
	}
}
void ShadowBeast::ShadowArm::updateStatusNormal(float deltatime)
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
void ShadowBeast::ShadowArm::updateElemPosition(HandElement& elem)
{
	elem._sprite->setPosition(
		_rootOfArm + elem._lenght * GVector2(cos(elem._angle), sin(elem._angle))
		);
}


/* Shadow Mouth */

GVector2 ShadowBeast::ShadowMouth::explose_position[12] = 
{
	GVector2(96, 64),
	GVector2(-96, 64),
	GVector2(32, 64),
	GVector2(-32, 64),
	GVector2(32, 0),
	GVector2(-32, 0),
	GVector2(32, -64),
	GVector2(-32, -64),
	GVector2(32, -128),
	GVector2(-32, -128),
	GVector2(32, -192),
	GVector2(-32, -192)
};
void ShadowBeast::ShadowMouth::init()
{
	_hitpoint = 32;
	_score = 10000;
	_sprite = SpriteManager::getInstance()->getSprite(eID::SHADOW_MOUTH);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_MOUTH, "mouth_opening1"));
	_sprite->setScale(SCALE_FACTOR);
	_sprite->setPosition(_startposition);

	_animation = new Animation(_sprite, 0.33f);
	_animation->addFrameRect(eID::SHADOW_MOUTH, "mouth_opening1", "mouth_opening2", "mouth_opening3", "mouth_opening4", "mouth_opening5", "mouth_opening4", "mouth_opening3", "mouth_opening2", "mouth_opening1",  "mouth_closed3", "mouth_closed2", "mouth_closed1", "mouth_closed2", "mouth_closed3", NULL);

	_flagCanShoot = true;
	_explosion = nullptr;
	_flagBigExplosion = true;


	_moulder = SpriteManager::getInstance()->getSprite(eID::SHADOW_MOUTH);
	_moulder->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::SHADOW_MOUTH, "moudler"));
	_moulder->setScale(SCALE_FACTOR);
	_moulder->setOrigin(GVector2(0.5f, 1.0f));
	_moulder->setPosition(_startposition + GVector2(0, -120) + GVector2(-1, 3));
	_moulder->setZIndex(0.0f);
	_flagMoudle = false;
}
void ShadowBeast::ShadowMouth::updateStatusNormal(float deltatime)
{
	if (_animation != nullptr)
	{
		_animation->update(deltatime);
	}
	if (_animation->getIndex() == 4)
	{
		if (_flagCanShoot == true)
		{
			shoot();
		}
	}
	else if (_animation->getIndex() == 9)
	{
		_flagCanShoot = true;
	}

}
void ShadowBeast::ShadowMouth::updateStatusBurst(float deltatime)
{
	if (this->_explosion == nullptr)
	{
		this->_explosion = new Explosion(2);
		this->_explosion->init();
		this->_explosion->setScale(SCALE_FACTOR);
		if (_flagBigExplosion == true)
		{
			_explosion->setPosition(this->getPosition());
		}
	}
	else
	{
		this->_explosion->update(deltatime);
		if (this->_explosion->getStatus() == eStatus::DESTROY)
		{
			_explosion->release();
			if (_flagBigExplosion == true)
			{
				_explosion->init();
				_explosion->setStatus(eStatus::NORMAL);
				this->_explosion->setScale(SCALE_FACTOR);
				SoundManager::getInstance()->Stop(eSoundId::BACKGROUND_STAGE2);
				SoundManager::getInstance()->Play(eSoundId::DESTROY_BOSS);
				_flagBigExplosion = false;
			}
			else
			{
				delete _explosion;
				_explosion = nullptr;
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
}
void ShadowBeast::ShadowMouth::update(float deltatime)
{
	if (this->getStatus() == eStatus::NORMAL)
	{
		this->updateStatusNormal(deltatime);
		if (this->getHitpoint() <= 0)
		{
			this->setStatus(eStatus::BURST);
		}
	}
	else if (this->getStatus() == eStatus::BURST)
	{
		this->updateStatusBurst(deltatime);
	}
}

void ShadowBeast::ShadowMouth::drawBillExplosion(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	for (int i = 0; i < 12; i++)
	{
		_explosion->setPosition(this->getPosition() + explose_position[i]);
		_explosion->draw(spriteHandle, viewport);
	}
}
void ShadowBeast::ShadowMouth::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	if (_flagMoudle == true)
	{
		_moulder->render(spriteHandle, viewport);
	}
	if (this->getStatus() == eStatus::NORMAL)
	{
		if (_animation != nullptr)
		{
			_animation->draw(spriteHandle, viewport);
		}
	}
	else if (this->getStatus() == eStatus::BURST)
	{
		if (this->_explosion != nullptr)
		{
			if (this->_flagBigExplosion == true)
			{
				this->_explosion->draw(spriteHandle, viewport);
			}
			else
			{
				this->drawBillExplosion(spriteHandle, viewport);
				_flagMoudle = true;
			}
		}
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
	int index = _animation->getIndex();
	if ( index >= 2 && index <= 6)
	{
		auto basebound = BaseObject::getBounding();
		basebound.left += 38 * this->getScale().x;
		basebound.right -= 38 * this->getScale().x;
		basebound.top += 14 * this->getScale().y;
		return basebound;
	}
	return RECT();
}
void ShadowBeast::ShadowMouth::shoot()
{
	BulletManager::insertBullet(new BeastBullet(this->getPosition() + GVector2(- 32, -32), D3DXToDegree( M_PI + M_PI_2 / 3)));
	BulletManager::insertBullet(new BeastBullet(this->getPosition() + GVector2(0, -32), D3DXToDegree(M_PI + 0)));
	BulletManager::insertBullet(new BeastBullet(this->getPosition() + GVector2(+ 32, -32), D3DXToDegree(M_PI - M_PI_2 / 3)));
	_flagCanShoot = false;
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
