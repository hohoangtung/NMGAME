#ifndef __NS_FRAMEWORK__
#define __NS_FRAMEWORK__

#define _USE_MATH_DEFINES

#include <d3d9.h>		// d3d9.lib
#include <d3dx9.h>		// d3dx9.lib
#include <dinput.h>		// dinput8.lib, dxguid.lib
#include <dsound.h>		// dsound.lib

#include <windows.h>
#include <exception>
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "..\debug.h"
#include "utils.h"
using namespace std;

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 448
#define SCALE_FACTOR 2.0f

#define C_WHITE D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)				// màu trắnng
#define COLOR_KEY D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)				// màu khi mà load hình nó bỏ qua > trong suốt
enum eID
{
	BILL			= 0,			// Main character.
	REDCANNON		= 1,
	SOLDIER			= 2,
	FALCON			= 3,
	AIRCRAFT		= 4,
	EXPLOSION		= 5,
	RIFLEMAN		= 6,
	BRIDGE			= 7,
	QUADEXPLODE		= 8,
	MAPSTAGE1		= 9,
	BULLET			= 10,
	WALL_TURRET		= 11,
	LAND			= 12,				// Đất đi được.
	CREATOR			= 13,

	// Từ 14 đến 16 dùng cho intro
	BLANK			= 14,
	MENU			= 15,
	YELLOWFALCON	= 16,
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
	HOLDING			= (1 << 17),
	SWIMING			= (1 << 18),
	DIVING			= (1 << 19),
	HIDDEN			= (1 << 20),
	WT_LEFT_30 = (1 << 20),
	WT_LEFT_60 = (1 << 21),
	WT_LEFT_150 = (1 << 22),
	WT_LEFT_120 = (1 << 23),
	WT_UP = (1 << 24),
	WT_DOWN = (1 << 25),
	WT_RIGHT = (1 << 26),
	WT_RIGHT_30 = (1 << 27),
	WT_RIGHT_60 = (1 << 28),
	WT_RIGHT_120 = (1 << 29),
	WT_RIGHT_150 = (1 << 30),
	WAITING			=(1<<31),
};

enum eLandType
{
	GRASS,
	WATER
};

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
	BILL_BULLET		= (1 << 0),
	ENEMY_BULLET	= (1 << 1),
	NORMAL_BULLET   = (1 << 2),
	L_BULLET		= (1 << 3),
	F_BULLET		= (1 << 4),
	S_BULLET		= (1 << 5),
	R_BULLET		= (1 << 6),
	M_BULLET		= (1 << 7),
};

enum eSoundId
{
	// Nhạc nền map 1
	BACKGROUND_STAGE1,					// Done.

	// Nhạc nền map 2
	BACKGROUND_STAGE2,

	// Nhạc nền map 3
	BACKGROUND_STAGE3,

	// Đạn va chạm xe tank, cannon.
	ATTACK_CANNON,					// Done.

	// Bắn đạn thường
	BASE_BULLET_FIRE,					// Done.

	// Bắn đạn Lửa (F)
	FBULLET_FIRE,					// Done.

	// Bắn đạn Chùm (S)
	SBULLET_FIRE,					// Done.

	// Bắn đạn đạn máy (M)
	MBULLET_FIRE,					// Done.

	// Bắn đạn la-ze  (L)
	LBULLET_FIRE,					// Done.

	// ?
	BOOM,

	// Tiếng rè rè của boss màn 1
	BOSS_SOUNG1,

	// Thằng Bill chết
	DEAD,

	// Giết boss
	DESTROY_BOSS,

	// Cầu nổ
	DESTROY_BRIDGE,					// Done.

	// Giết thằng lính
	DESTROY_ENEMY,					// Done. Mà tiếng này nhỏ quá không nghe.

	// Nổ cầu
	BRIDGE_BURN,					// Done.

	// Ăn đồ, thay đạn.
	EAT_ITEM,						// Done.

	// ?
	ENEMY_ATTACK,

	// Thua
	GAME_OVER,

	// Giết boss cuối ở màn 8
	GAMEOVER_BOSS,

	// Intro
	INTRO,

	// Bill nhảy
	JUMP_SOUND,						// Done.

	// Giết boss
	PASS_BOSS,

	// Phá đảo.
	WINGAME
};

typedef D3DXVECTOR3 GVector3;
typedef D3DXVECTOR2 GVector2;
#define VECTOR2ZERO GVector2(0.0f, 0.0f)
#define VECTOR2ONE  GVector2(1.0f, 1.0f)

#define NS_FRAMEWORK		namespace FrameWork

#define NS_FRAMEWORK_BEGIN	{

#define NS_FRAMEWORK_END	}

#define US_FRAMEWORK		using namespace FrameWork;

#ifndef SAFE_DELETE

#define SAFE_DELETE(p) \
if(p) \
{\
	delete (p); \
	p = nullptr; \
} \

#endif // !SAFE_DELETE



#endif // !__NS_FRAMEWORK__

