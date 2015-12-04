#ifndef __LIFEUI_H__
#define __LIFEUI_H__

#include "EmptyObject.h"
#include "../FrameWork/define.h"

#define GAP 10

class LifeUI : public EmptyObject
{
public:
	LifeUI(GVector2 position, int number);
	~LifeUI();

	virtual void init();
	virtual void update(float deltatime);
	virtual void draw(LPD3DXSPRITE, Viewport*);
	virtual void release();

	void setLifeNumber(int number);
	int getLifeNumber();

private:
	vector<Sprite*> _listIcons;
};

#endif // !__LIFEUI_H__
