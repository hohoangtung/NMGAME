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