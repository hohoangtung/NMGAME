#ifndef _BASE_ENEMY_H
#define _BASE_ENEMY_H

#include "BaseObject.h"

class BaseEnemy : public BaseObject 
{
public:
	BaseEnemy(eID id) : BaseObject(id) {}
	
	int getHitpoint();
	void setHitpoint(int);
    int getScore();
	void setScore(int);

	virtual ~BaseEnemy() {}

private:
	int _hitpoint;
	int _score;
};
#endif