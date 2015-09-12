
#ifndef __SPIRTE_H__
#define __SPIRTE_H__

#include "define.h"
#include "DeviceManager.h"
#include "Texture.h"

using namespace std;

NS_FRAMEWORK
NS_FRAMEWORK_BEGIN

// định nghĩa các hành động cơ bản khi vẽ hình. chưa đầy đủ. nhưng có thể sử dụng được.
class Sprite
{
public:
	
	// filePath: đường dẫn hình ảnh
	// count: số frame ảnh trên một file
	// SPR: số frame trên một dòng
	Sprite(LPD3DXSPRITE spriteHandle, LPWSTR filePath, int count = 1, int SPR = 1);
	//Sprite(int width, int height, int count = 1, int SPR = 1);
	~Sprite();
	void render(LPD3DXSPRITE spriteHandle);							// Vẽ hình
	void release();
	void setIndex(int);			
	void next();				// duyệt các frame theo thứ tự mặc định
	GVector3 getPosition();
	void setPosition(int x, int y, int z = 1);
	void setPosition(GVector3 vector);
	
private:
	Texture				_Texture;
	//LPD3DXSPRITE		_spriteHandle;			//=> cái này giống spritebatch

	GVector3			_position;				// không được gán trực tiếp mà phải gọi setPosition();

	int		_count;					// số frame của hình, sao khi khởi tạo thì không được sửa
	int		_spriteperrow;			// số frame trên một dòng, sao khi khởi tạo thì không được sửa
	int		_rowCount;				// số dòng frame, sao khi khởi tạo thì không được sửa

	//không được gán trực tiếp, mà dùng setIndex(int)
	int			_index;

	// Chứa cặp X, Y là toạ độ theo index của một frame so với hình lớn
	GVector2	_curFrame;			
	void setCurrentFrame();			// đừng care. chỉ để cho đoạn code gọn

	int		_frameWidth;			// Chiều rộng của một frame hình
	int		_frameHeight;			// Chiều cao của một frame hình

	RECT	_frameRect;			//top, bottom, right, left
	void setFrameRect();
	
	RECT	_bound;
};

NS_FRAMEWORK_END
#endif // !__SPIRTE_H__
