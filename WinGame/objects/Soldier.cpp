#include "Soldier.h"

bool jumped = false;

Soldier::Soldier() : BaseEnemy(eID::SOLDIER) {
	this->setStatus(RUNNING);
}

Soldier::~Soldier() {}

void Soldier::init()
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::SOLDIER);
	_sprite->setFrameRect(0, 0, 32.0f, 16.0f);
	this->setPosition(600, 300);
	GVector2 v(-SOLDIER_SPEED, 0);
	GVector2 a(0, 0);

	this->setScale(SCALE_FACTOR);

	this->setHitpoint(SOLDIER_HITPOINT);
	this->setScore(SOLDIER_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Gravity", new Gravity(GVector2(0, -400), (Movement*)(this->getComponent("Movement")))));

	auto collisionBody = new CollisionBody(this);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Soldier::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Soldier::onCollisionEnd);

	_animations[RUNNING] = new Animation(_sprite, 0.15f);
	_animations[RUNNING]->addFrameRect(eID::SOLDIER, "run_01", "run_02", "run_03", "run_04", "run_05", "run_06", NULL);

	_animations[SHOOTING] = new Animation(_sprite, 0.15f);
	_animations[SHOOTING]->addFrameRect(eID::SOLDIER, "shoot_01", "shoot_02", NULL);

	_animations[JUMPING] = new Animation(_sprite, 0.15f);
	_animations[JUMPING]->addFrameRect(eID::SOLDIER, "jump_01", NULL);

	_animations[FALLING] = new Animation(_sprite, 0.15f);
	_animations[FALLING]->addFrameRect(eID::SOLDIER, "jump_01", NULL);

	_animations[LAYING_DOWN] = new Animation(_sprite, 0.15f);
	_animations[LAYING_DOWN]->addFrameRect(eID::SOLDIER, "lay_down_01", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::SOLDIER, "die_01", NULL);

	_stopwatch = new StopWatch();

	jump();
}

void Soldier::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	this->_sprite->render(spritehandle, viewport);
	_animations[this->getStatus()]->draw(spritehandle, viewport);
}

void Soldier::release()
{

}

IComponent* Soldier::getComponent(string componentName)
{
	return _listComponent.find(componentName)->second;
}

void Soldier::update(float deltatime)
{
	Gravity *gravity = (Gravity*)this->getComponent("Gravity");
	Movement *movement = (Movement*)this->getComponent("Movement");

	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}

	_animations[this->getStatus()]->update(deltatime);
}

void Soldier::changeDirection()
{
	_sprite->setScaleX(-this->getScale().x);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Soldier::onCollisionBegin(CollisionEventArg* collision_eventt) {

}

void Soldier::onCollisionEnd(CollisionEventArg* collision_event) {

}

BaseObject* prevObject;
float Soldier::checkCollision(BaseObject * object, float dt)
{
	auto collisionBody = (CollisionBody*)_listComponent["CollisionBody"];

	if (object->getId() == eID::LAND || object->getId() == eID::BOX || object->getId() == eID::BRIDGE)
	{
		eDirection direction;

		if (collisionBody->checkCollision(object, direction, dt))
		{
			if (direction == eDirection::TOP && this->getVelocity().y < 0)
			{
				auto gravity = (Gravity*)this->_listComponent["Gravity"];
				gravity->setStatus(eGravityStatus::SHALLOWED);

				this->setStatus(RUNNING);

				auto move = (Movement*)this->_listComponent["Movement"];
				move->setVelocity(GVector2(move->getVelocity().x, 0));
				prevObject = object;
			}
		}
		else if (prevObject == object)
		{
			prevObject = nullptr;

			auto gravity = (Gravity*)this->_listComponent["Gravity"];
			gravity->setStatus(eGravityStatus::FALLING__DOWN);
			int randNum = rand() % 2;
			if (randNum == 1)
			{
				if (!this->isInStatus(eStatus::JUMPING))
					jump();
			}
			else
				changeDirection();
		}
	}
	else
	{
		collisionBody->checkCollision(object, dt);
	}

	return 0.0f;
}

void Soldier::jump() 
{
	this->setStatus(FALLING);
	Movement *move = (Movement*)this->getComponent("Movement");
	move->setVelocity(GVector2(move->getVelocity().x, SOLDIER_JUMP_VELOCITY));
}

GVector2 Soldier::getVelocity()
{
	auto move = (Movement*)this->_listComponent["Movement"];
	return move->getVelocity();
}

