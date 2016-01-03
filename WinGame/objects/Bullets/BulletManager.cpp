
#include "BulletManager.h"

list<Bullet*> BulletManager::_listBullet;

void BulletManager::init()
{

}

float BulletManager::checkCollision(BaseObject * object, float dt)
{
	// Tùng: sử chỉ cho kiểm tra va chạm với bill và land
	eID id = object->getId();
	if (id != BILL && id != LAND)
	{
		return 0.0f;
	}
	for (Bullet* bullet : _listBullet)
	{
		if (bullet == nullptr)
		{
			_listBullet.remove(bullet);
			break;
		}
		else
		{
			//if (dynamic_cast<BaseEnemy*>(object) && bullet->isEnemyBullet())
			//{
			//	continue;
			//}
			eID id = object->getId();
			if (id != BILL && id != LAND)
			{
				continue;
			}
			bullet->checkCollision(object, dt);
		}
	}
	return 0.0f;
}

void BulletManager::update(float deltatime)
{
	for (Bullet* bullet : _listBullet)
	{
		if (bullet == nullptr)
		{
			_listBullet.remove(bullet);
			break;
		}
		else
		{
			bullet->update(deltatime);
			if (bullet->getStatus() == eStatus::DESTROY)
			{
				bullet->release();
				_listBullet.remove(bullet);
				delete bullet;
				bullet = nullptr;
				break;
			}
		}
	}
}
void BulletManager::draw(LPD3DXSPRITE spriteHandle, Viewport* viewport)
{
	for (Bullet* bullet : _listBullet)
	{
		if (bullet == nullptr)
		{
			_listBullet.remove(bullet);
			break;
		}
		else
		{
			bullet->draw(spriteHandle, viewport);
		}
	}
}
void BulletManager::release()
{
	for (Bullet* bullet : _listBullet)
	{
		if (bullet == nullptr)
		{
			_listBullet.remove(bullet);
			break;
		}
		else
		{
			_listBullet.remove(bullet);
			break;
			bullet->release();
			delete bullet;
			bullet = nullptr;
		}
	}
}

void BulletManager::insertBullet(Bullet* bullet)
{
	if (bullet == nullptr)
		throw new exception("bullet cannot null");
	bullet->init();
	_listBullet.push_back(bullet);
}


RECT BulletManager::getBounding()
{
	return RECT();
}

//treat as bullet
BulletManager::BulletManager() : BaseObject(eID::BULLET)
{
}

BulletManager::~BulletManager()
{
}
