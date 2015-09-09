#include "Surface.h"


Surface::Surface(void)
{
	m_lpBackBuffer = NULL;
	m_lpSurface = NULL;
}
Surface::Surface(const Surface & surface)
{
	m_lpSurface = LPDIRECT3DSURFACE9(surface.m_lpSurface);
	m_lpBackBuffer = LPDIRECT3DSURFACE9(surface.m_lpBackBuffer);
}

Surface::~Surface(void)
{
}
void Surface::UnloadSurface()
{

}



void Surface::LoadSurfaceFromFile(LPDIRECT3DDEVICE9 _lpDirectDevice, D3DCOLOR ColorKey, LPCWSTR fileName)
{
	D3DXIMAGE_INFO _infoImage;
	HRESULT hr;

	hr = D3DXGetImageInfoFromFile(fileName, &_infoImage);
	if (FAILED(hr))
		throw;

	hr = _lpDirectDevice->CreateOffscreenPlainSurface(
		_infoImage.Width,
		_infoImage.Height,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_lpSurface,
		0);			//tạo font nền cho surface

	if (FAILED(hr))
		throw;

	hr = D3DXLoadSurfaceFromFile(
		m_lpSurface,
		0,
		0,
		fileName,
		0,
		D3DX_DEFAULT,
		ColorKey,
		0);						//copy file vào surface

	if (FAILED(hr))
		throw;
	
	hr = _lpDirectDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_lpBackBuffer);
	if (FAILED(hr))
		throw;
}

void Surface::RenderSurface(LPDIRECT3DDEVICE9 _lpDirectDevice, RECT * SourceRect, RECT * DesRect)
{
	// vẽ surface lên back buffer
	_lpDirectDevice->StretchRect(
		m_lpSurface,
		SourceRect,
		m_lpBackBuffer,
		DesRect,
		D3DTEXF_NONE);
	
}