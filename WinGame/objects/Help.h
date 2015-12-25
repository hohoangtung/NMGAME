


#ifndef __HELP_H__
#define __HELP_H__


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

#endif // !__HELP_H__
