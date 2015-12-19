#ifndef __FIRE__
#define __FIRE__
#include "BaseObject.h"
#include "IComponent.h"
#include "CollisionBody.h"
#include "PlayScene.h"
#include "../FrameWork/Animation.h"
using namespace std;

#define HORIZONTAL_VELOC		GVector2(100.0f, 0.0f)		// vận tốc ngang
#define HORIZONTAL_VELOC_PRE	GVector2(-100.0f, 0.0f)		// vận tốc ngang
#define RANGEMOVE_FIRE			200
class Fire:public BaseObject
{
public:
	Fire(GVector2);

	void init();
	void update(float);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	float checkCollision(BaseObject*, float);
	IComponent* getComponent(string);
	~Fire();

	class SingleFire :public BaseObject
	{
	public:
		SingleFire(GVector2, int);

		void init();
		void update(float);
		void draw(LPD3DXSPRITE, Viewport*);
		void release();
		GVector2 getVelocity();
		void onCollisionBegin(CollisionEventArg*);
		void onCollisionEnd(CollisionEventArg*);
		float checkCollision(BaseObject*,float);
		IComponent* getComponent(string);

		~SingleFire();
		
	private:
		map<string, IComponent*> _listComponent;
		Animation* _animation;
		GVector2 _startposition;
		int _veloc;
	};
private:
	map<string, IComponent*> _listComponent;
	
	GVector2 _startposition;
	BaseObject* _singleFire1;
	BaseObject* _singleFire2;
	


};
#endif
