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
	FALCON,
	AIRCRAFT,
	EXPLOSION,
	MAPSTAGE1
};

enum eStatus
{

	NORMAL			= 0,					// 00000 = 0	
	MOVING_LEFT		= (1 << 0),				// 00001 = 2^0
	MOVING_RIGHT	= (1 << 1),				// 00010 = 2^1
	JUMPING			= (1 << 2),				// 00100 = 2^2
	LAYING_DOWN		= (1 << 3),				// 01000 = 2^3
	RUNNING			= (1 << 4),				// 10000 = 2^4
	LOOKING_UP		= (1 << 5),				// 2^5
	SHOOTING		= (1 << 6),
	DESTROY			= (1 << 7),
	BURST			= (1 << 8),
	EXPLORE			= (1 << 9)
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)
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
