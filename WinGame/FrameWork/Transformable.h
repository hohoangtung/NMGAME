#ifndef __TRANSFORMABLE_H__
#define __TRANSFORMABLE_H__

#include "define.h"

class Transformable
{
public:
	Transformable();
	~Transformable();

	GVector2 virtual getPosition();
	float virtual getPositionX();
	float virtual getPositionY();

	void virtual setPosition(GVector3 vector);
	void virtual setPosition(int x, int y, int z);
	void virtual setPosition(GVector2 position);
	void virtual setPosition(int x, int y);
	void virtual setPositionX(float x);
	void virtual setPositionY(float y);

	GVector2 virtual getScale();
	void virtual setScale(GVector2 scale);
	void virtual setScale(float scale);
	void virtual setScaleX(float sx);
	void virtual setScaleY(float sy);

	float virtual getRotate();
	void virtual setRotate(float degree);

	GVector2 virtual getOrigin();
	void virtual setOrigin(GVector2 origin);

	void virtual setZIndex(int z);
	int virtual getZIndex();

protected:
	GVector2			_position;
	GVector2			_scale;
	float				_rotate;				// theo độ 0-360
	GVector2			_origin;				// gốc của Transform, dùng để xoay, scale (anchor: điểm neo)
	int					_zIndex;
};

#endif // !__TRANSFORMABLE_H__
