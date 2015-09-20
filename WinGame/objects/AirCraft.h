

#ifndef __AIRCRAFT_H__
#define __AIRCRAFT_H__

#include "BaseObject.h"
#include "IComponent.h"
#include "..\FrameWork\Animation.h"
#include "PlayScene.h"
#include "Explosion.h"
#include <map>
using namespace std;

enum eAirCraftType
{
	B,
	F,
	L,
	M,
	R,
	S,
	I,
};

// use value
#define AIRCRAFT_FORCE			GVector2(25.0f, 150.0f)		// góc ném viên đạn
#define AIRCRAFT_GRAVITY		GVector2(0.0f, -200.0f)
// demo value 
#define START_POSITION			GVector2(200.0f, 200.0f)	// vị trí khởi tạo
#define HORIZONTAL_VELOC		GVector2(90.0f, 0.0f)		// vận tốc ngang
#define AIRCRAFT_FREQUENCY		0.9f						// tần số
#define AIRCRAFT_AMPLITUDE		GVector2(0, 100.0f)			// biên độ

[event_receiver(native)]
class AirCraft : public BaseObject, public IControlable
{
public:
	AirCraft(GVector2 pos, GVector2 hVeloc, GVector2 ampl, float freq, eAirCraftType type);
	~AirCraft();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	void updateInput(float deltatime) override;

	IComponent* getComponent(string componentName);
	void keypressed	(KeyEventArg*);

	int getType();
private:
	map<string, IComponent*> _listComponent;
	BaseObject* _explosion;
	eAirCraftType _type;
	Animation* _animation;
	void initExplosion();
	void updateExplosion(float deltatime);

	// some init value
	GVector2	_beginPosition;
	GVector2	_horizontalVeloc;
	GVector2	_amplitude;
	float		_frequence;
};


#endif // !__AIRCRAFT_H__
