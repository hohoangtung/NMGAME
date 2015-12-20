#ifndef __OBJECTCREATOR_H__
#define __OBJECTCREATOR_H__

#include "../FrameWork/define.h"
#include "../FrameWork/StopWatch.h"
#include "BaseObject.h"
#include "EmptyObject.h"
#include "Soldier.h"

[event_receiver(native)]
class ObjectCreator : public EmptyObject
{
public:

	/*
	T?o s? l??ng object ? v? trí trong kho?ng th?i gian
		@position: v? trí
		@type: lo?i object
		@direction: -1 là trái, 1 là ph?i
		@timeCreate: kho?ng th?i gian gi?a m?i l?n t?o
		@number: s? l??ng object (-1 là vô h?n)
	*/
	ObjectCreator(GVector2 position, int width, int height, eID type, int direction = -1, float timeCreate = 1000, int number = -1);

	~ObjectCreator();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	virtual void release();

	virtual float checkCollision(BaseObject* object, float dt);
	RECT getBounding() override;
private:

	BaseObject* getObject(eID id);
	void deleteObject();

	StopWatch* _stopWatch;
	vector<BaseObject*> _listObjects;

	eID _createType;
	float _timeCreate;
	int _number;
	int _counter;

	int _direction;
};

#endif // !__OBJECTCREATOR_H__
