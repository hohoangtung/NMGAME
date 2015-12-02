#include "BaseEnemy.h"
#include "..\FrameWork\Managers\SoundManager.h"

BaseEnemy::BaseEnemy(eID id) : BaseObject(id) {}
BaseEnemy::~BaseEnemy() {}
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

void BaseEnemy::dropHitpoint()
{
	_hitpoint--;
}

void BaseEnemy::dropHitpoint(int damage)
{
	_hitpoint -= damage;

}