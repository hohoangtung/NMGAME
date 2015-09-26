#ifndef _BASE_ENEMY_H
#define _BASE_ENEMY_H

#include "BaseObject.h"

class BaseEnemy : public BaseObject 
{
public:
	BaseEnemy(eID id) : BaseObject(id) {}
	
	int getHealth();
	void setHealth(int);
    int getScore();
	void setScore(int);

	virtual ~BaseEnemy() {}

private:
	int _health;
	int _score;
};
#endif