#ifndef _BASE_ENEMY_H
#define _BASE_ENEMY_H

#include "../BaseObject.h"
#include "../CollisionBody.h"

#define ENEMY_GRAVITY 500

class BaseEnemy : public BaseObject 
{
public:
	BaseEnemy(eID);
	
	int getHitpoint();
	void setHitpoint(int);
    int getScore();
	void setScore(int);
	virtual void dropHitpoint();
	virtual void dropHitpoint(int damage);
	
	void setStatus(eStatus status) override;
	virtual ~BaseEnemy();
protected:
	void gainScore();
	int _hitpoint;
	int _score;
};
#endif