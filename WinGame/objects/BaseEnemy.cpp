#include "BaseEnemy.h"

int BaseEnemy::getHealth() 
{
	return _health;
}

void BaseEnemy::setHealth(int health)
{
	_health = health;
}

void BaseEnemy::setScore(int score)
{
	_score = score;
}

int BaseEnemy::getScore()
{
	return _score;
}