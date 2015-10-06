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
	this->setPosition(600, 500);
	GVector2 v(-SOLDIER_SPEED, 0);
	GVector2 a(0, 0);

	this->setHitpoint(SOLDIER_HITPOINT);
	this->setScore(SOLDIER_SCORE);

	this->_listComponent.insert(pair<string, IComponent*>("Movement", new Movement(a, v, this->_sprite)));
	this->_listComponent.insert(pair<string, IComponent*>("Gravity", new Gravity(GVector2(0, -120), (Movement*)(this->getComponent("Movement")))));

//	this->setPhysicsBodyType(ePhysicsBody::MAN);
	auto collisionBody = new CollisionBody(this);
//	collisionBody->setPhysicsObjects(ePhysicsBody::LAND);
	_listComponent["CollisionBody"] = collisionBody;

	__hook(&CollisionBody::onCollisionBegin, collisionBody, &Soldier::onCollisionBegin);
	__hook(&CollisionBody::onCollisionEnd, collisionBody, &Soldier::onCollisionEnd);

	_animations[RUNNING] = new Animation(_sprite, 0.15f);
	_animations[RUNNING]->addFrameRect(eID::SOLDIER, "run_01", "run_02", "run_03", "run_04", "run_05", "run_06", NULL);

	_animations[SHOOTING] = new Animation(_sprite, 0.15f);
	_animations[SHOOTING]->addFrameRect(eID::SOLDIER, "shoot_01", "shoot_02", NULL);

	_animations[JUMPING] = new Animation(_sprite, 0.15f);
	_animations[JUMPING]->addFrameRect(eID::SOLDIER, "jump_01", NULL);

	_animations[LAYING_DOWN] = new Animation(_sprite, 0.15f);
	_animations[LAYING_DOWN]->addFrameRect(eID::SOLDIER, "lay_down_01", NULL);

	_animations[DYING] = new Animation(_sprite, 0.15f);
	_animations[DYING]->addFrameRect(eID::SOLDIER, "die_01", NULL);

	_stopwatch = new StopWatch();
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
	switch (this->getStatus())
	{
	case RUNNING:
		// movement->setVelocity(GVector2(-30, 0));
		gravity->setStatus(SHALLOWED);
		if (this->_sprite->getPositionX() < 500){
			/*jumped = true;
			movement->setVelocity(GVector2(movement->getVelocity().x, 60));
			this->setStatus(JUMPING);*/
			this->_sprite->setPositionX(500);
			changeDirection();
		}
		break;
	case JUMPING:
		gravity->setStatus(FALLING__DOWN);
		if (this->_sprite->getPositionY() < 400) {
			this->_sprite->setPositionY(400);
			this->setStatus(RUNNING);
		}
		break;
	case SHOOTING:
		gravity->setStatus(SHALLOWED);
		movement->setVelocity(GVector2(0, 0));
		break;
	case LAYING_DOWN:
		gravity->setStatus(SHALLOWED);
		movement->setVelocity(GVector2(0, 0));
		break;
	//case DYING:
	//	gravity->setStatus(FALLING__DOWN);
	//	movement->setVelocity(GVector2(0, 60));
	//	break;
	}
	for (auto it : _listComponent)
	{
		it.second->update(deltatime);
	}
	//if (_stopwatch->isTimeLoop(8000.0f))
	//{
	//	Movement *move = (Movement*)this->getComponent("Movement");
	//	GVector2 v = move->getVelocity();
	//	move->setVelocity(GVector2(-v.x, -v.y));
	//}
	_animations[this->getStatus()]->update(deltatime);
}

void Soldier::changeDirection()
{
	_sprite->setScaleX(-1);
	Movement *movement = (Movement*)this->getComponent("Movement");
	movement->setVelocity(GVector2(-movement->getVelocity().x, 0));
}

void Soldier::onCollisionBegin(CollisionEventArg* collision_eventt) {

}

void Soldier::onCollisionEnd(CollisionEventArg* collision_event) {

}