#include "GameTime.h"
US_FRAMEWORK

GameTime* GameTime::_instance = nullptr;

GameTime::GameTime(void)
{
}

GameTime::GameTime(TimeSpan& elapsedGameTime)
{
	this->_elapsedGameTime = elapsedGameTime;
}

GameTime::~GameTime(void)
{
}

GameTime* GameTime::getInstance()
{
	if (_instance == nullptr)
		_instance = new GameTime();
	return _instance;
}

void GameTime::release()
{
	delete _instance;
	_instance = NULL;
}

void GameTime::init()
{
	QueryPerformanceFrequency(&this->_Query);

	this->_freQuery = (float)_Query.QuadPart / 10000000;		
	//10000000 mean use microsecond.
	// if want to use milisecond use 1000
	// if want to use second use 1

	QueryPerformanceCounter(&_Query);
	startTicks = lastTicks = _Query.QuadPart;
	_totalGameTime = (TimeSpan) 0;
}

void GameTime::resetLastTick()
{
	lastTicks = 0;
	curTicks = 0;
	_totalGameTime = (TimeSpan) 0;
}

void GameTime::updateGameTime()
{
	QueryPerformanceCounter(&_Query);
	curTicks = _Query.QuadPart;
	if ((UINT64) ((float)(curTicks - lastTicks )/ _freQuery)   <  TimeSpan::TicksPerMilisecond * 16)
	{
		return;
	}
	auto gt = ((float)(curTicks - lastTicks)) / _freQuery;
	this->setTotalGameTime( _totalGameTime + gt);
	this->setElapsedGameTime(
		TimeSpan((UINT64) gt)
		);
	lastTicks = curTicks;
	
}

float GameTime::getElapsedGameTime()
{
	return this->_elapsedGameTime.getMiliSeconds();
}
float GameTime::getTotalGameTime()
{
	return this->_totalGameTime.getMiliSeconds();
}
void GameTime::setElapsedGameTime(TimeSpan& elapsedgametime)
{
	this->_elapsedGameTime = elapsedgametime;
}
void GameTime::setTotalGameTime(TimeSpan& totalGametime)
{
	this->_totalGameTime = totalGametime;
}