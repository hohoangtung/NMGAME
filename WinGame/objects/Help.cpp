
#include "Help.h"

void Help::initStackKey()
{
	_stack_Key.push_back(DIK_X);
	_stack_Key.push_back(DIK_Z);

	_stack_Key.push_back(DIK_LEFT);
	_stack_Key.push_back(DIK_RIGHT);
	_stack_Key.push_back(DIK_UP);
	_stack_Key.push_back(DIK_DOWN);


}
void Help::update(float deltatime)
{
	_radian += deltatime / 1000;
	_opacity = abs(sin(_radian));		// -1 <= sin <= 1
	_message->setOpacity(_opacity);

	if (_stack_Key.empty() == true)
	{
		_message->setString("Contra game for Se102 G13 project");
		return;
	}

	switch (_stack_Key.back())
	{
	case DIK_LEFT:
		_message->setString("Press left Arrow for turn left");
		break;
	case DIK_RIGHT:
		_message->setString("Press right Arrow for turn right");
		break;
	case DIK_UP:
		_message->setString("Press up Arrow for aim up");
		break;
	case DIK_DOWN:
		_message->setString("Press down Arrow for turn lie down");
		break;
	case DIK_X:
		_message->setString("Press key X for jump");
		break;
	case DIK_Z:
		_message->setString("Press key z Arrow for fire");
		break;

	default:
		_message->setString("Well done Enjoy Your Game");
		break;
	}
}

void Help::onKeyPressed(KeyEventArg* key_event)
{
	if (_stack_Key.empty())
	{
		return;
	}
	if (key_event->_key == _stack_Key.back())
	{
		_stack_Key.pop_back();
	}
}
void Help::init()
{
	initStackKey();
	_radian = 0.0f;
	_opacity = 0.0f;
	_message = new TextSprite(eID::FONTFULL, "", GVector2(150, 30));
	_message->init();
	//_message->setScale(SCALE_FACTOR);
	__hook(&InputController::__eventkeyPressed, _input, &Help::onKeyPressed);

}
void Help::updateInput(float deltatime)
{

}

void Help::draw(LPD3DXSPRITE spriteHandle)
{
	_message->draw(spriteHandle);
}
Help::Help()
{
}

Help::~Help()
{
}


/// credit

void Credit::init()
{
	_message = new TextSprite(eID::FONTFULL, "", GVector2(150, 30));
	_message->init();
	_message->setScale(1.5f);
	_stopwatch = new StopWatch();
	current = 0;
	_radian = 0.0f;
	_opacity = 0.0f;
}
void Credit::update(float deltatime)
{
	_radian += deltatime / 1000;
	_opacity = abs(sin(_radian));		// -1 <= sin <= 1
	_message->setOpacity(_opacity);

	if (_stopwatch->isTimeLoop(3141.5))
	{
		current++;
	}
	this->_message->setString(_listInfo[current % 10]);
}

void Credit::draw(LPD3DXSPRITE spriteHandle)
{
	_message->draw(spriteHandle);

}

string Credit::_listInfo[10] =
{
	"Do An Nhap Mon Game 2015",
	"Giang Vien Nguyen Phuong Anh",
	"Nhom Sinh Vien Thuc Hien",
	"Ho Hoang Tung 13521005",
	"Luu The Vinh 13521043",
	"Lam Tuan Anh 13520020",
	"Nguyen Tan Luan 13520476",
	"Huynh Ngoc Thanh Phu 13520630",
	"Thank For Playing",
	"Press Q for Return"
};

Credit::Credit()
{

}
Credit::~Credit()
{

}