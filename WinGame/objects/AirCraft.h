/*
*	author: Ho Hoang Tung
*	
*	AirCraft: lớp dùng để tạo một đối tượng máy bay có thể thay đổi kiểu đạn của nhân vật.
*/

#ifndef __AIRCRAFT_H__
#define __AIRCRAFT_H__

#include "../FrameWork/Animation.h"
#include "BaseObject.h"
#include "IComponent.h"
#include "Explosion.h"
#include "Scenes/PlayScene.h"
#include "Item.h"

// use value
#define AIRCRAFT_FORCE			GVector2(50.0f, 280.0f)		// góc ném viên đạn
#define AIRCRAFT_GRAVITY		GVector2(0.0f, -200.0f)
// demo value 
#define START_POSITION			GVector2(200.0f, 350.0f)	// vị trí khởi tạo
#define AIRCRAFT_HORIZONTAL_VELOC		GVector2(230.0f, 0.0f)		// vận tốc ngang
#define AIRCRAFT_FREQUENCY		0.9f						// tần số
#define AIRCRAFT_AMPLITUDE		GVector2(0, 64.0f)			// biên độ

class AirCraft : public BaseObject
{
public:
	/* Contructor:
	*		@pos: vị trí bắt đầu.
	*		@hVeloc: gia tốc tuyến tính, tạo một phần chuyển động thăng.
	*		@ampl: biên độ của chuyển động tuần hoàn.
	*		@freq: tần số góc của chuyển động tuần hoàn.
	*		@type: loại máy bay, là loại đạn mà nhân vật có thể lấy được.
	*/
	AirCraft(GVector2 pos, GVector2 hVeloc, GVector2 ampl, float freq, eAirCraftType type);
	~AirCraft();

	// Các phương thức kế thừa từ BaseObject.
	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	GVector2 getVelocity() override;

	IComponent* getComponent(string componentName);
	eAirCraftType getType();

	// Set Explored sẽ gán biến _explored thành true, sử dụng khi nhân vật đã chạm vào máy bay lúc nổ.
	void setExplored();

	// Kiểm tra va chạm.
	float checkCollision(BaseObject* object, float dt);

private:
	map<string, IComponent*> _listComponent;
	BaseObject* _explosion;
	BaseObject* _item;
	eAirCraftType _type;
	Animation* _animation;
	void initExplosion();


	// some init value
	GVector2	_beginPosition;
	GVector2	_horizontalVeloc;
	GVector2	_amplitude;
	float		_frequence;

	bool _explored;
	void checkifOutofScreen();
	void updateHiding();

	void updateExploring(float deltatime);

	bool _verticalflag;
};



#endif // !__AIRCRAFT_H__
