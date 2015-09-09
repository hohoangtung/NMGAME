
#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include "define.h"
#include "Graphics.h"

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

class DeviceManager
{
public:

	~DeviceManager(void);
	static DeviceManager* getInstance();
	static void release();
	void clearScreen();
	void present();
	void Init(Graphics);

	LPDIRECT3DDEVICE9 getDevice();
	LPDIRECT3DSURFACE9 getSurface();

private:
	static DeviceManager* _instance;
	LPDIRECT3D9			_pD3d;
	LPDIRECT3DDEVICE9	_pDevice;	
	LPDIRECT3DSURFACE9	_surface;
	DeviceManager(void);
};
typedef DeviceManager *pDeviceManager;
NS_FRAMEWORK_END

#endif // !__DEVICE_MANAGER_H__
