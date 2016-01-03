#include "BaseEnemy.h"
#include "../../FrameWork/Managers/SoundManager.h"
#include "../Score.h"
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
	if (_hitpoint <= 0)
		gainScore();
}

void BaseEnemy::gainScore()
{
	Score::plusScore(this->_score);
}

void BaseEnemy::dropHitpoint(int damage)
{
	_hitpoint -= damage;
	if (_hitpoint <= 0)
		gainScore();
}

void BaseEnemy::setStatus(eStatus status)
{
	if (status == eStatus::DESTROY)
	{
		SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
	}
	BaseObject::setStatus(status);
}