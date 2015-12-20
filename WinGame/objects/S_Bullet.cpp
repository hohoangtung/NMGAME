#include "S_Bullet.h"

SBullet::SBullet(GVector2 startPosition, float degree)
	: Bullet(startPosition, eBulletType(BILL_BULLET | S_BULLET), degree)
{

}

void SBullet::init()
{
	_damage = 1;

	_childBullet[0] = this->initChildBullet(_startPosition, _degree);
	_childBullet[1] = this->initChildBullet(_startPosition, _degree + FIRST_WAVE_ANGLE);
	_childBullet[2] = this->initChildBullet(_startPosition, _degree - FIRST_WAVE_ANGLE);
	_childBullet[3] = this->initChildBullet(_startPosition, _degree + SECOND_WAVE_ANGLE);
	_childBullet[4] = this->initChildBullet(_startPosition, _degree - SECOND_WAVE_ANGLE);

	_first_wave = new StopWatch();
	_second_wave = new StopWatch();
}

Bullet* SBullet::initChildBullet(GVector2 startposition, float _degre)
{
	auto child = new Bullet(startposition, eBulletType(BILL_BULLET | S_BULLET), _degre);
	child->init();
	child->getSprite()->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "s_bullet"));
	child->setDamge(this->_damage);
	return child;
}

float SBullet::checkCollision(BaseObject* object, float dt)
{
	for (Bullet* bullet : _childBullet)
	{
		if (bullet == nullptr)
			continue;
		if (bullet->getStatus() == eStatus::DESTROY)
			continue;
		bullet->checkCollision(object, dt);
	}
	return 0.0f;
}

void SBullet::update(float deltatime)
{
	bool destroy = true;

	if (_first_wave!= NULL &&_first_wave->isStopWatch(FIRST_ILLU_TIME))
	{
		this->initFirstWave();
		delete _first_wave;
		_first_wave = NULL;
	}
	if (_second_wave != NULL && _second_wave->isStopWatch(SECOND_ILLU_TIME))
	{
		this->initSecondWave();
		delete _second_wave;
		_second_wave = NULL;
	}
	for (int i = 0; i < 5; i++)
	{
		if (_childBullet[i] == nullptr)
			continue;
		_childBullet[i]->update(deltatime);
		if (destroy == true && _childBullet[i]->getStatus() != eStatus::DESTROY)
		{
			destroy = false;
		}
	}
	if (destroy == true)
	{
		this->setStatus(eStatus::DESTROY);
	}
	this->deleteChilds();
}

// Phân thân lần thứ nhất, tạo ra 2 bullet ở chỉ số 1 và 2
void SBullet::initFirstWave()
{

	//this->_childBullet[1]->setStatus(eStatus::NORMAL);
	//this->_childBullet[2]->setStatus(eStatus::NORMAL);
}

// Phân thân lần thứ nhất, tạo ra 2 bullet ở chỉ số 3 và 4
void SBullet::initSecondWave()
{

	//this->_childBullet[3]->setStatus(eStatus::NORMAL);
	//this->_childBullet[4]->setStatus(eStatus::NORMAL);

}

// Kiểm tra nếu đạn con hết hiệu lực thì huỷ đối tượng.
void SBullet::deleteChilds()
{
	for (int i = 0; i < 5; i++)
	{
		// kiểm tra nếu là destroy thì loại khỏi list
		if (_childBullet[i] == nullptr)
			continue;
		if (_childBullet[i]->getStatus() != eStatus::DESTROY)
			continue;
		_childBullet[i]->release();

		delete _childBullet[i];
		_childBullet[i] = nullptr;
		
	}
}

void SBullet::release()
{
	Bullet::release();
	for (Bullet* bullet : _childBullet)
	{
		SAFE_DELETE(bullet);
	}
	SAFE_DELETE(_first_wave);
	SAFE_DELETE(_second_wave);
}

void SBullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	for (Bullet* bullet : _childBullet)
	{
		// Ngẫu nhiên 5 lần vẽ thì không vẽ một lần tạo hiệu ứng nhấp nháy
		if (rand() % 5 == 0)
			return;
		if (bullet == nullptr)
			continue;
		if (bullet->getStatus() != eStatus::NORMAL)
			continue;

		bullet->draw(spriteHandle, viewport);
	}
}

SBullet::~SBullet()
{
}
