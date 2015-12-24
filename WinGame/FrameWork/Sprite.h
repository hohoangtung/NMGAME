#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <list>
#include "define.h"
#include "DeviceManager.h"
#include "Texture.h"
#include "Viewport.h"
#include "Transformable.h"

using namespace std;

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

class Sprite : public Transformable
{
public:
	/*
		@filePath: đường dẫn hình ảnh
		@totalFrames: số frame ảnh trên một file
		@cols: số frame trên một dòng
	Nếu không truyền số frame, số cột (= 1) sẽ vẽ hết hình. Để vẽ một vị trí cụ thể trên hình thì dùng setFrameRect().
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

	void setPosition(float x, float y, float z = 1);
	void setPosition(GVector3 vector);
	void setPosition(GVector2 position);
	void setPositionX(float x);
	void setPositionY(float y);

	void setScale(GVector2 scale);
	void setScale(float scale);
	void setScaleX(float sx);
	void setScaleY(float sy);

	void setRotate(float degree);
	
	void setOrigin(GVector2 origin);

	void setZIndex(float z);

	RECT getBounding();

	void setFrameRect(RECT rect);
	void setFrameRect(float top, float right, float bottom, float left);
	void setFrameRect(float x, float y, int width, int height);
	RECT getFrameRect();
	RECT getFrameRectByIndex(int index);
	/*
	Chuyển qua frame kế tiếp
	*/
	void nextFrame();

	/*
	Truyền thứ tự frame cụ thể
	*/
	void setIndex(int index);

	/*
	Lấy chiều ngang của frame
	*/
	int getFrameWidth();

	/*
	Lấy chiều dọc của frame
	*/
	int getFrameHeight();

	/*
	Lấy chiều ngang của nguyên tấm hình
	*/
	int getTextureWidth();

	/*
	Lấy chiều dọc của nguyên tấm hình
	*/
	int getTextureHeight();

	void drawBounding(bool draw);

	void setOpacity(float opacity);
	float getOpacity();

	/*
	Màu vẽ sprite
	*/
	void setColor(D3DXCOLOR color);
	D3DXCOLOR getColor();

private:
	Texture				_texture;
	float				_opacity;						// từ 0.0f đến 1.0f thôi
	D3DXCOLOR			_color;

	RECT				_bound;

	int					_totalFrames;					// tổng số frame
	int					_columns;						// số cột
	int					_index;
	GVector2			_currentFrame;					// frame hiện tại

	RECT				_frameRect;
	int					_frameWidth;
	int					_frameHeight;
	int					_textureWidth;
	int					_textureHeight;

	void setFrameRect();
	void setCurrentFrame();
	void updateBounding();

	GVector2 rotatePointAroundOrigin(GVector2 point, float angle, GVector2 origin);

	//surface để vẽ bounding
	LPDIRECT3DSURFACE9	_surface;
	bool				_isDrawBounding;
};

NS_FRAMEWORK_END

#endif // !__SPRITE_H__
