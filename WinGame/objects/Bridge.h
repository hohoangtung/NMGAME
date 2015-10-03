#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include "BaseObject.h"
#include "Explosion.h"

// nếu sửa max_wave thì phải sửa _matrixIndex
#define MAX_WAVE 4		// số lần nổ
#define DELAYTIME 500.0f

[event_receiver(native)] 
class Bridge : public BaseObject, public IControlable
{
public:
	Bridge();
	~Bridge();

	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE, Viewport*);
	void release();
	void updateInput(float deltatime);

	void burst(float);
	void testExplose(KeyEventArg*);

	void setPosition(GVector2 position);
	GVector2 getPosition();
private:
	static int _matrixIndex[2][MAX_WAVE * 2];
	StopWatch* _stopwatch;
	Transformable *_transform;
	int _wave;
private:
	class QuadExplose : public BaseObject
	{
	public:
		QuadExplose(GVector2);
		void init();
		void update(float deltatime);
		void draw(LPD3DXSPRITE, Viewport*);
		void release();
		void setPosition(GVector2 position);
		GVector2 getPosition();
		void reset();
	private:
		Transformable* _transform;
		BaseObject* _explosion1;
		BaseObject* _explosion2;
		BaseObject* _explosion3;
		BaseObject* _explosion4;
		float _timer;

	};
	QuadExplose* _explode;
};
#endif // !__BRIDGE_H__
