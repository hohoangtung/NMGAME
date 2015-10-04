#include "BaseEnemy.h"

int BaseEnemy::getHitpoint() 
{
	return _hitpoint;
}

void BaseEnemy::setHitpoint(int hitpoint)
{
	_hitpoint = hitpoint;
}

void BaseEnemy::setScore(int score)
{
	_score = score;
}

int BaseEnemy::getScore()
{
	return _score;
}

void BaseEnemy::onCollisionBegin(CollisionEventArg* collision_event) {

}

void BaseEnemy::onCollisionEnd(CollisionEventArg* collision_event) {

}