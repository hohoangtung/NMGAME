
#include "L_Bullet.h"

LGun::LBullet::LBullet(GVector2 startPosition, float degree)
	: Bullet(startPosition, eBulletType(BILL_BULLET | L_BULLET), degree)
{
}

void LGun::LBullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "l_bullet"));

	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	if (this->_degree == RIGHT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(0.0f);
	}
	else if (this->_degree == LEFT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(0.0f);
	}
	else if (this->_degree == TOPRIGHT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(-45);
	}
	else if (this->_degree == TOPLEFT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(45);
	}
	else if (this->_degree == BOTRIGHT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(45);
	}
	else if (this->_degree == BOTLEFT_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(-45);
	}
	else if (this->_degree == TOP_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(90);
	}
	else if (this->_degree == BOTTOM_SHOOT_ANGLE)
	{
		this->_sprite->setRotate(90);
	}

	GVector2 begin_veloc = Bullet::initveloc(L_BULLET_SPEED);

	auto movement = new Movement(GVector2(0, 0), begin_veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));

	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

void LGun::LBullet::update(float deltatime)
{

	Bullet::update(deltatime);
}
void LGun::LBullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	Bullet::draw(spriteHandle, viewport);
}
void LGun::LBullet::release()
{
	Bullet::release();
}
LGun::LBullet::~LBullet()
{
}

LGun::LGun(GVector2 startPosition, float degree)
	:Bullet(startPosition, eBulletType(BILL_BULLET | L_BULLET), degree)
{
}

void LGun::init()
{
	_damage = 4;
	// Tính lại _startposition
	_childBullet[0] = this->initChildBullet(_startPosition, _degree);

	float lenght = _childBullet[0]->getSprite()->getFrameWidth();
	float height = _childBullet[0]->getSprite()->getFrameHeight();
	auto pos1 = generatePosition(_startPosition, _degree, lenght, height);
	_childBullet[1] = this->initChildBullet(pos1, _degree);

	auto pos2 = generatePosition(pos1, _degree, lenght, height);
	_childBullet[2] = this->initChildBullet(pos2, _degree);

}

GVector2 LGun::generatePosition(GVector2 startposition, float degree, float R, float H)
{
	GVector2 rs;
	float x = R * sin(D3DXToRadian(degree));
	float y = R * cos(D3DXToRadian(degree));
	if (int(y) != 0 && int(x) != 0)
	{
		if (cos(D3DXToRadian(degree)) > 0)
		{
			y += H *abs(sin(D3DXToRadian(degree)));
		}
		else
		{
			y -= H*  abs(sin(D3DXToRadian(degree)));
		}
	}
	rs.x = startposition.x + x;
	rs.y = startposition.y + y;
	return rs;
}

Bullet* LGun::initChildBullet(GVector2 startposition, float _degre)
{
	auto child = new LBullet(startposition, _degre);
	child->init();
	child->getSprite()->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "l_bullet"));
	child->setDamge(this->_damage);
	return child;
}

float LGun::checkCollision(BaseObject* object, float dt)
{
	for (Bullet* bullet : _childBullet)
	{
		if (bullet == nullptr)
			continue;
		if (bullet->getStatus() != eStatus::NORMAL)
			continue;
		bullet->checkCollision(object, dt);
	}
	return 0.0f;
}

void LGun::update(float deltatime)
{
	bool destroy = true;
	for (int i = 0; i < 3; i++)
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

// Kiểm tra nếu đạn con hết hiệu lực thì huỷ đối tượng.
void LGun::deleteChilds()
{
	for (int i = 0; i < 3; i++)
	{
		// kiểm tra nếu là destroy thì loại khỏi list
		if (_childBullet[i] == NULL)
			continue;
		if (_childBullet[i]->getStatus() != eStatus::DESTROY)
			continue;

		_childBullet[i]->release();

		delete _childBullet[i];
		_childBullet[i] = nullptr;
		
	}
}
void LGun::release()
{
	for (Bullet* bullet : _childBullet)
	{
		SAFE_DELETE(bullet);
	}
	SAFE_DELETE(_sprite);

}

void LGun::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	for (Bullet* bullet : _childBullet)
	{
		// Ngẫu nhiên 5 lần vẽ thì không vẽ một lần tạo hiệu ứng nhấp nháy
		if (rand() % 5 == 0)
			return;
		if (bullet == NULL)
			continue;
		if (bullet->getStatus() != eStatus::NORMAL)
			continue;

		bullet->draw(spriteHandle, viewport);
	}
}
LGun::~LGun()
{
}