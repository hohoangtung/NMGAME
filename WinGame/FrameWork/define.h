#ifndef __NS_FRAMEWORK__
#define __NS_FRAMEWORK__

#define _USE_MATH_DEFINES

#include <d3d9.h>		// d3d9.lib
#include <d3dx9.h>		// d3dx9.lib
#include <dinput.h>		// dinput8.lib, dxguid.lib

#include <windows.h>
#include <exception>
#include <math.h>

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define C_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)				// màu trắnng
#define COLOR_KEY D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)				// màu khi mà load hình nó bỏ qua > trong suốt

enum eID
{
	FLOWER,
	BILL,
	MARIO,
	REDCANNON,
	SOLDIER,
	FALCON
};

enum eStatus
{
	NORMAL			= 0,				// 00000 = 0
	MOVING_LEFT		= 1,				// 00001 = 2^0
	MOVING_RIGHT	= 2,				// 00010 = 2^1
	JUMPING			= 4,				// 00100 = 2^2
	LAYING_DOWN		= 8,				// 01000 = 2^3
	RUNNING			= 16,				// 10000 = 2^4
	LOOKING_UP		= 32,				// 2^5
	SHOOTING		= 64				// 2^6
};

enum eDirection
{
	TOP				= 1,
	BOTTOM			= 2,
	LEFT			= 4,
	RIGHT			= 8
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;

#define NS_FRAMEWORK		namespace FrameWork

#define NS_FRAMEWORK_BEGIN	{

#define NS_FRAMEWORK_END	}

#define US_FRAMEWORK		using namespace FrameWork;

#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
} \

#endif // !__NS_FRAMEWORK__
