#include "Animation.h"

Animation::Animation(int totals, int cols, int frameW, int frameH)
{
	_totalFrames = totals;
	_columns = cols;
	_frameWidth = frameW;
	_frameHeight = frameH;
	_timer = 0;
	_timeAnimate = 0;
	_startFrame = 0;
	_endFrame = totals - 1;

	this->setIndex(_startFrame);

	if(_totalFrames <= 1)
		_canAnimate = false;
	else
		_canAnimate = true;
}

Animation::~Animation()
{
}

void Animation::createAnimate(int startFrame, int endFrame, float timeAnimate)
{
	_startFrame = startFrame;
	_endFrame = endFrame;
	_timeAnimate = timeAnimate;

	this->setIndex(_startFrame);

	if (_endFrame - _startFrame <= 0)
		_canAnimate = false;
	else
		_canAnimate = true;
}

void Animation::nextFrame()
{
	this->setIndex(_index + 1);
}

int Animation::getCurrentFrame()
{
	return _index;
}

void Animation::setIndex(int index)
{
	_index = index;

	this->setCurrentFrame();
}

void Animation::setCurrentFrame()
{
	if (_index > _endFrame)
		_index = _index % (_endFrame - _startFrame + 1);

	this->_currentFrame.x = _index % _columns;
	this->_currentFrame.y = _index / _totalFrames;

	this->setFrameRect();
}

void Animation::setFrameRect()
{
	this->_frameRect.left = (long)_currentFrame.x * _frameWidth;
	this->_frameRect.right = _frameRect.left + _frameWidth;
	this->_frameRect.top = (long)_currentFrame.y * _frameHeight;
	this->_frameRect.bottom = _frameRect.top + _frameHeight;
}

RECT Animation::getFrameRect()
{
	return _frameRect;
}

void Animation::update(float dt)
{
	if (!_canAnimate)
		return;

	_timer += dt / 1000;
	if (_timer >= _timeAnimate)
	{
		this->nextFrame();
		_timer = 0;
	}
	
}

void Animation::setTimeAnimate(float time)
{
	_timeAnimate = time;
}

float Animation::getTimeAnimate()
{
	return _timeAnimate;
}

int Animation::getFrameWidth()
{
	return _frameWidth;
}

int Animation::getFrameHeight()
{
	return _frameHeight;
}

void Animation::start()
{
	if (_canAnimate == false)
		_canAnimate = true;
}

void Animation::stop()
{
	if (_canAnimate == true)
		_canAnimate = false;
}

void Animation::canAnimate(bool can)
{
	if (_canAnimate != can)
		_canAnimate = can;
}
