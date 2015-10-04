#ifndef _BASE_ENEMY_H
#define _BASE_ENEMY_H

#include "BaseObject.h"
#include "CollisionBody.h"

class BaseEnemy : public BaseObject 
{
public:
	BaseEnemy(eID id) : BaseObject(id) {}
	
	int getHitpoint();
	void setHitpoint(int);
    int getScore();
	void setScore(int);
	void onCollisionBegin(CollisionEventArg*);
	void onCollisionEnd(CollisionEventArg*);

	virtual ~BaseEnemy() {}

private:
	int _hitpoint;
	int _score;
};
#endif