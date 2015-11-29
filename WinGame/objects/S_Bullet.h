

#ifndef __S_BULLET__
#define __S_BULLET__




#include "Bullet.h"
#include "..\FrameWork\StopWatch.h"

#define S_BULLET_SPEED 700

// Thời điểm phân thân lần thứ nhất.
#define FIRST_ILLU_TIME			100.0f

// Thời điểm phân thân lần thứ hai.
#define SECOND_ILLU_TIME		280.0f

// Góc mở của lớp đạn thứ nhất
#define FIRST_WAVE_ANGLE	11.0f

// Góc mở của lớp đạn thứ hai
#define SECOND_WAVE_ANGLE	22.0f

class SBullet : public Bullet
{
public:
	SBullet(GVector2 startPosition, float degree);
	~SBullet();

	void init() override;
	void update(float deltatime) override;
	void draw(LPD3DXSPRITE, Viewport*) override;
	void release() override;
	float checkCollision(BaseObject* object, float dt) override;
private:

	StopWatch* _first_wave;
	StopWatch* _second_wave;

	// Quy định 0 là đạn ở giữa, 1 và 2 là hai đạn nhỏ hai bên, 3 và 4 là hai đạn lớn ngoài cùng.
	Bullet* _childBullet[5];

	void deleteChilds();

	Bullet* initChildBullet(GVector2 startposition, float _degree);

	// Phân thân lần thứ nhất, tạo ra 2 bullet nhỏ hai bên
	void initFirstWave();

	// Phân thân lần thứ nhất, tạo ra 2 bullet lớn ngoài cùng
	void initSecondWave();
};
#endif // !__S_BULLET__
