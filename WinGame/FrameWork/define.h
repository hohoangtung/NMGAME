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
	MARIO
};
enum eStatus
{
	NORMAL
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
