
#include "StopWatch.h"

US_FRAMEWORK

StopWatch::StopWatch()
{
	_stopwatch = 0.0f;
	_isStart = false;
	_isFinish = false;
}

StopWatch::~StopWatch()
{
}

bool StopWatch:: isFinish()
{
	return this->_isFinish;
}
// milisecond
bool StopWatch::isTimeLoop(float time)
{
	float _totalgametime = GameTime::getInstance()->getTotalGameTime();
	if (_isStart == false)
	{
		_stopwatch = time + _totalgametime;
		_isStart = true;
		return false;
	}
	float delta = _stopwatch - _totalgametime;
	if (delta <= 0)
	{
		_stopwatch = time + delta + _totalgametime;
		return true;
	}
	return false;
}

bool StopWatch::isStopWatch(float time)
{
	if (_isFinish == true)
		return false;
	float _totalgametime = GameTime::getInstance()->getTotalGameTime();

	if (_isStart == false)
	{
		_stopwatch = time + _totalgametime;
		_isStart = true;
		return false;
	}
	float delta = _stopwatch - _totalgametime;
	if (delta <= 0)
	{
		_isFinish = true;
		return true;
	}
	return false;
}
void StopWatch::restart()
{
	_isStart = false;
	_isFinish = false;
}

void StopWatch::timeLoopAction(float milisecond, pFunction action)
{
	if (isTimeLoop(milisecond))
	{
		(*action) ();
	}
}
