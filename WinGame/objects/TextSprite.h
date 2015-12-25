

#ifndef __TEXTSPRITE__
#define __TEXTSPRITE__

#include "BaseObject.h"

class TextSprite : public BaseObject
{
public:
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle, Viewport* viewport);
	void draw(LPD3DXSPRITE spriteHandle);
	void release();

	void setString(string text);

	void setPosition(GVector2 position);
	GVector2 getPosition();

	TextSprite(eID type, string text, GVector2 position);
	~TextSprite();

private:
	eID _type;
	string _text;
	GVector2 _position;

	int _columns;
	int rows;
	int _framewidth;
};
#endif // !__TEXTSPRITE__
