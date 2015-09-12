
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "define.h"
#include "DeviceManager.h"
#include "Viewport.h"

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

#define C_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

class Texture
{
public:
	Texture(void);
	~Texture(void);

	HRESULT loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color = C_WHITE);
	
	/*
	render texture không có tranform
	*/
	void render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position);
	
	/*
	render texture với các tham số tranform, gốc tọa độ top-left
		@position: vị trí
		@scale: 
		@rotate: góc xoay theo số độ (0-360)
		@origin: điểm neo để transform
		@zIndex: độ sâu
	*/
	void render(LPD3DXSPRITE spriteHandle, RECT* srcRect, GVector2 position, GVector2 scale, float rotate, GVector2 origin, int zIndex = 1);
	
	/*
	render texture với các tham số tranform, trong viewport
		@viewport: 
		@position: vị trí
		@scale:
		@rotate: góc xoay theo số độ (0-360)
		@origin: điểm neo để transform
		@zIndex: độ sâu
	*/
	void render(LPD3DXSPRITE spriteHandle, RECT* srcRect, Viewport viewport, GVector2 position, GVector2 scale, float rotate, GVector2 origin, int zIndex = 1);

	void setColor(D3DXCOLOR);
	int getWidth();
	int getHeight();

private:
	LPDIRECT3DTEXTURE9	_texture;
	D3DXIMAGE_INFO		_imageInfo;

	D3DXCOLOR			_color;
};

typedef Texture* pTexture;

NS_FRAMEWORK_END

#endif // !__TEXTURE_H__

