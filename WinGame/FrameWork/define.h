#ifndef __NS_FRAMEWORK__
#define __NS_FRAMEWORK__

#define _USE_MATH_DEFINES

#include <d3d9.h>		// d3d9.lib
#include <d3dx9.h>		// d3dx9.lib
#include <dinput.h>		// dinput8.lib, dxguid.lib

#include <windows.h>
#include <exception>
#include <math.h>
#include <string>
#include "..\debug.h"
#include "utils.h"
using namespace std;

#define WINDOW_WIDTH 560
#define WINDOW_HEIGHT 448
#define SCALE_FACTOR 2.0f

#define C_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)				// màu trắnng
#define COLOR_KEY D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)				// màu khi mà load hình nó bỏ qua > trong suốt
enum eID
{
	FLOWER,			// Mario flower. Just for test.
	BILL,			// Main character.
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
	BULLET,
	LAND,				// Đất đi được.
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

	// Trạng thái huỷ, lúc này, đối tượng không update, không draw, ở scene kiểm tra nếu phát hiện trạng thái này thì huỷ đối tượng.
	DESTROY			= (1 << 7),

	// Trạng thái nổ, lúc này có thể vẽ các đám cháy, vụ nổ.
	BURST			= (1 << 8),

	// Dùng cho aircraft, trạng thái thể hiện đang nổ để chuyển từ cái máy bay thành đồ tiếp đạn.
	EXPLORING		= (1 << 9),

	// Dùng cho aircraft, trạng thái đồ tiếp đạn.
	EXPLORED		= (1 << 10),

	DYING			= (1 << 11),
	AIMING_UP		= (1 << 12),
	AIMING_DOWN		= (1 << 13),
	HIDING			= (1 << 14),
	EXPOSING		= (1 << 15),
	FALLING			= (1 << 16),
	HOLDING			= (1 << 17)
};

enum ePhysicsBody
{
	NOTHING			= 0,
	TOP_EDGE		= (1 << 0),
	LEFT_EDGE		= (1 << 1),
	RIGHT_EDGE		= (1 << 2),
	BOTTOM_EDGE		= (1 << 3)
};

enum eLandType
{
	GRASS,
	WATER
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

enum eAirCraftType
{
	B,
	F,
	L,
	M,
	R,
	S,
	I,
};

enum eBulletType
{
	BILL_BULLET		= 0,
	ENEMY_BULLET	= (1 << 0),
	NORMAL_BULLET   = (1 << 1),
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)
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
