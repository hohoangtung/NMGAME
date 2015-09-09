#include "TimeSpan.h"

US_FRAMEWORK

TimeSpan::TimeSpan(void)
{
}

TimeSpan::~TimeSpan(void)
{
}

TimeSpan::TimeSpan(UINT64 ticks)
{
	this->_ticks = ticks;
}

float TimeSpan::getMiliSeconds()
{
	return ((float) _ticks )/ TicksPerMilisecond;
}

UINT64 TimeSpan::getTicks()
{
	return _ticks;
}

TimeSpan TimeSpan::operator+(TimeSpan ts)
{
	return TimeSpan(this->_ticks + ts._ticks);
}

TimeSpan TimeSpan::operator+(UINT64 ts)
{
	return TimeSpan(*this + TimeSpan(ts));
}