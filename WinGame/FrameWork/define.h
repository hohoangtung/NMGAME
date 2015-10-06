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

#define WINDOW_WIDTH 560
#define WINDOW_HEIGHT 448
#define SCALE_FACTOR 2.0f

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
	RIFLEMAN,
	BOX,
	BRIDGE,
	QUADEXPLODE,
	MAPSTAGE1,
	BULLET
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
	EXPLORE			= (1 << 9),
	DYING			= (1 << 10),
	AIMING_UP		= (1 << 11),
	AIMING_DOWN		= (1 << 12),
	HIDING			= (1 << 13),
	EXPOSING		= (1 << 14),
	FALLING			= (1 << 15),
	HOLDING			= (1 << 16)
};

enum ePhysicsBody
{
	NOTHING			= 0,
	TOP_EDGE		= (1 << 0),
	LEFT_EDGE		= (1 << 1),
	RIGHT_EDGE		= (1 << 2),
	BOTTOM_EDGE		= (1 << 3)
};

#define ALL_EDGES (TOP_EDGE | LEFT_EDGE | RIGHT_EDGE | BOTTOM_EDGE)

enum eDirection
{
	NONE			= 0,
	TOP				= 1,
	BOTTOM			= 2,
	LEFT			= 4,
	RIGHT			= 8
};


enum eBulletType
{
	NORMAL_BULLET,
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
