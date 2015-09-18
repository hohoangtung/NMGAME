#include "Animation.h"
#include "..\debug.h"

Animation::Animation(Sprite * spriteSheet, float timeAnimate)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;

	_canAnimate = true;
	_totalFrames = 0;
	_index = 0;
	_timer = 0;

	this->setIndex(0);
}

Animation::Animation(Sprite * spriteSheet, int totalFrames, int cols, float timeAnimate)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;
	_canAnimate = true;
	_totalFrames = totalFrames;
	_index = 0;
	_timer = 0;

	int frameW = spriteSheet->getTextureWidth() / cols;
	int frameH = spriteSheet->getTextureHeight() * cols / totalFrames;

	for (int i = 0; i < totalFrames; i++)
	{
		int x = i % cols;
		int y = i / cols;

		this->addFrameRect(x * frameW, y * frameH, frameW, frameH);
	}

	_currentRect = _frameRectList[_index];
}

Animation::~Animation()
{
	// dont release sprite
}

void Animation::nextFrame()
{
	this->setIndex(_index + 1);
}

void Animation::setIndex(int index)
{
	if (index == _index || _totalFrames == 0)
		return;

	_index = index;

	if (_index >= _totalFrames)
		_index = _index % _totalFrames;

	_currentRect = _frameRectList[_index];
}

void Animation::update(float dt)
{
	if (!_canAnimate)
		return;

	_timer += dt / 1000;
	if (_timer >= _timeAnimate)
	{
		this->nextFrame();
		_timer -= _timeAnimate;				// không thể gán bằng 0. vì như vậy là làm tròn. sẽ có sai số
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

void Animation::addFrameRect(RECT rect)
{
	//nếu là rect đầu tiên thì set current luôn
	if (_frameRectList.empty())
		_currentRect = rect;

	_frameRectList.push_back(rect);
	_totalFrames = _frameRectList.size();
}

void Animation::addFrameRect(float left, float top, int width, int height)
{
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;

	this->addFrameRect(rect);
}

void Animation::addFrameRect(float left, float top, float right, float bottom)
{
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	this->addFrameRect(rect);
}

void Animation::draw(LPD3DXSPRITE spriteHandle, Viewport * viewport)
{
	_spriteSheet->setFrameRect(_currentRect);
	_spriteSheet->render(spriteHandle, viewport);
}
