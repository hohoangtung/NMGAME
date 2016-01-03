
#include "F_Bullet.h"

FBullet::FBullet(GVector2 startPosition,  float degree)
	: Bullet(startPosition, eBulletType(BILL_BULLET|F_BULLET), degree)
{
}

void FBullet::init()
{
	_damage = 1;

	_sprite = SpriteManager::getInstance()->getSprite(eID::BULLET);
	_sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::BULLET, "f_bullet"));

	this->setPosition(_startPosition);
	this->setScale(SCALE_FACTOR);

	GVector2 begin_veloc = Bullet::initveloc(F_BULLET_SPEED);

	auto movement = new Movement(GVector2(0, 0), begin_veloc, _sprite);
	_componentList.insert(pair<string, IComponent*>("Movement", movement));
	
	float radian = this->initRadian();

	auto roundmovement = new RoundMovement(ROUND_RADIUS, ROUND_FREQUENCE, radian, _sprite);
	_componentList["RoundMovement"] = roundmovement;
	//auto circlemovement = new CircleMovement(150.0f, 1.0f, _sprite);
	//_componentList["RoundMovement"] = circlemovement;
	auto collisionBody = new CollisionBody(this);
	_componentList.insert(pair<string, IComponent*>("CollisionBody", collisionBody));

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Bullet::onCollisionBegin);
}

float FBullet::initRadian()
{
	if (this->_degree == RIGHT_SHOOT_ANGLE)
	{
		return -M_PI_4;
	}
	else if (this->_degree == LEFT_SHOOT_ANGLE)
	{
		return M_PI - M_PI_4;
	}
	else if (this->_degree == TOPRIGHT_SHOOT_ANGLE)
	{
		return 0;
	}
	else if (this->_degree == TOPLEFT_SHOOT_ANGLE)
	{
		return 0;
		//return M_PI / 2;
	}
	else if (this->_degree == BOTRIGHT_SHOOT_ANGLE)
	{
		return 0;
		//return -M_PI / 2;
	}
	else if (this->_degree == BOTLEFT_SHOOT_ANGLE)
	{
		return 0;
		//return M_PI / 2;
	}
	else if (this->_degree == TOP_SHOOT_ANGLE)
	{
		return M_PI_2;
	}
	else if (this->_degree == BOTTOM_SHOOT_ANGLE)
	{
		return 0.0;			// not handle yet.
	}
}

void FBullet::update(float deltatime)
{
	Bullet::update(deltatime);
}
void FBullet::draw(LPD3DXSPRITE spriteHandle, Viewport *viewport)
{
	// Ngẫu nhiên 5 lần vẽ thì không vẽ một lần tạo hiệu ứng nhấp nháy
	if (rand() % 5 == 0)
		return;
	Bullet::draw(spriteHandle, viewport);
}
void FBullet::release()
{
	Bullet::release();
	SAFE_DELETE(_animations);
}
FBullet::~FBullet()
{
}