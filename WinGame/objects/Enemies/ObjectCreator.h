#ifndef __OBJECTCREATOR_H__
#define __OBJECTCREATOR_H__

#include "../../FrameWork/define.h"
#include "../../FrameWork/StopWatch.h"
#include "../BaseObject.h"
#include "../EmptyObject.h"
#include "Soldier.h"

[event_receiver(native)]
class ObjectCreator : public EmptyObject
{
public:

	/*
	Tạo số lượng object ở vị trí trong khỏang thời gian
		@position: vị trí
		@type: lọai object
		@direction: -1 là trái, 1 là phải
		@timeCreate: khoảnng thời gian giữa mỗi lần tạo
		@number: số lượng object (-1 là vô hạn)
	*/
	ObjectCreator(GVector2 position, int width, int height, eID type, int direction = -1, float timeCreate = 1000, int number = -1);

	~ObjectCreator();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	virtual void release();

	virtual float checkCollision(BaseObject* object, float dt);
	RECT getBounding() override;

	vector<BaseObject*> getObjects();

	void setOnePerOne(bool enable);
	bool isOnePerOne();

	void setMaxNumber(int number);
	int getMaxNumber();

	void setMapType(eMapType type);
	eMapType getMapType();

private:

	BaseObject* getObject(eID id);
	void deleteObject();

	StopWatch* _stopWatch;
	vector<BaseObject*> _listObjects;

	eID _createType;
	float _timeCreate;
	int _number;
	int _counter;
	int _maxObject;

	int _direction;
	bool _isOnePerOne;

	eMapType _mapType;
};

#endif // !__OBJECTCREATOR_H__
