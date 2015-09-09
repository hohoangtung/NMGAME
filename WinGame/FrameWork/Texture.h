
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "define.h"
#include "DeviceManager.h"
NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

#define C_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

class Texture
{
public:
	Texture(void);
	~Texture(void);

	HRESULT loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color = C_WHITE);
	void render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position);

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

