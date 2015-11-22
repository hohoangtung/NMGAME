#ifndef _BASE_ENEMY_H
#define _BASE_ENEMY_H

#include "BaseObject.h"
#include "CollisionBody.h"

#define GRAVITY 500
class BaseEnemy : public BaseObject 
{
public:
	BaseEnemy(eID);
	
	int getHitpoint();
	void setHitpoint(int);
    int getScore();
	void setScore(int);

	virtual ~BaseEnemy();

protected:
	int _hitpoint;
	int _score;
};
#endif