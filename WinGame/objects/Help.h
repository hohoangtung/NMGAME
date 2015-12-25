


#ifndef __HELP_H__
#define __HELP_H__

#include "../FrameWork/StopWatch.h"
#include "TextSprite.h"
#include <vector>

[event_receiver(native)]
class Help : public IControlable
{
public:
	Help();
	~Help();
	void init();
	void update(float deltatime);

	void onKeyPressed(KeyEventArg* key_event);
	void updateInput(float deltatime) override;
	void draw(LPD3DXSPRITE spriteHandle);

private:
	vector<int> _stack_Key;
	float _radian;
	float _opacity;
	TextSprite* _message;
	void initStackKey();
	void initOpacity();
};


class Credit
{
public:
	Credit();
	~Credit();
	void init();
	void update(float deltatime);
	void draw(LPD3DXSPRITE spriteHandle);

private:
	static string _listInfo[];
	TextSprite* _message;
	StopWatch* _stopwatch;
	int current;
	float _radian;
	float _opacity;
};

#endif // !__HELP_H__
