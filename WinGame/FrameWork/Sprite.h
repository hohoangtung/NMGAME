#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <list>
#include "define.h"
#include "DeviceManager.h"
#include "Texture.h"
#include "Viewport.h"
#include "Animation.h"

using namespace std;

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

class Sprite
{
public:
	/*
		@filePath: đường dẫn hình ảnh
		@totalFrames: số frame ảnh trên một file
		@cols: số frame trên một dòng
	*/
	Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int totalFrames = 1, int cols = 1);

	~Sprite();

	/*
	
	*/
	void release();
	
	/*
	vẽ hình
	*/
	void render(LPD3DXSPRITE spriteHandle);

	/*
	vẽ hình với viewport
	*/
	void render(LPD3DXSPRITE spriteHandle, Viewport* viewport);

	GVector2 getPosition();
	float getPositionX();
	float getPositionY();

	void setPosition(int x, int y, int z = 1);
	void setPosition(GVector3 vector);
	void setPosition(GVector2 position);
	void setPositionX(float x);
	void setPositionY(float y);

	GVector2 getScale();
	void setScale(GVector2 scale);
	void setScale(float scale);
	void setScaleX(float sx);
	void setScaleY(float sy);

	float getRotate();
	void setRotate(float degree);
	
	GVector2 getOrigin();
	void setOrigin(GVector2 origin);

	void setZIndex(int z);
	int getZIndex();

	void update(float dt);

	RECT getBounding();

	void setVelocity(GVector2 vel);
	void setVelocity(float x, float y);
	void setVelocityX(float velX);
	void setVelocityY(float velY);

	void setAccelerate(GVector2 acc);
	void setAccelerate(float x, float y);
	void setAccelerateX(float accX);
	void setAccelerateY(float accY);

	Animation* getAnimation();

private:
	Texture				_texture;

	GVector2			_position;				// không được gán trực tiếp mà phải gọi setPosition();
	GVector2			_scale;
	float				_rotate;				// theo độ 0-360
	GVector2			_origin;				// gốc của sprite, dùng để xoay, scale (anchor: điểm neo)
	int					_zIndex;

	GVector2			_velocity;
	GVector2			_accelerate;

	RECT	_bound;
	void updateBounding();

	Animation* _animation;
};

NS_FRAMEWORK_END

#endif // !__SPRITE_H__
