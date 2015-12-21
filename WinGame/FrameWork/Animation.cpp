#include "Animation.h"
#include "..\debug.h"

Animation::Animation(Sprite * spriteSheet, float timeAnimate, bool loop)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;

	_canAnimate = true;
	_totalFrames = 0;
	_index = 0;
	_timer = 0;
	_valueFlashes = 0.5f;
	 
	this->setIndex(0);
	this->setLoop(loop);
}

Animation::Animation(Sprite * spriteSheet, int totalFrames, int cols, float timeAnimate)
{
	_spriteSheet = spriteSheet;
	_timeAnimate = timeAnimate;
	_canAnimate = true;
	_totalFrames = totalFrames;
	_index = 0;
	_timer = 0;
	_valueFlashes = 0.5f;

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
int Animation::getIndex()
{
	// 7ung -> use in Explostion::update
	return _index;
}
void Animation::setIndex(int index)
{
	if (index == _index || _totalFrames == 0)
		return;

	_index = index;

	if (_index >= _totalFrames)
		_index = _index % _totalFrames;
	
	_currentRect = _frameRectList[_index];

	if (!_isLoop && _index == _totalFrames - 1)
	{
		this->stop();
	}
}

void Animation::update(float dt)
{
	if (!_canFlashes && !_canAnimate)
		return;

	_timer += dt / 1000;
	if (_timer >= _timeAnimate)
	{
		if (_canAnimate)
			this->nextFrame();

		_timer -= _timeAnimate;				// không thể gán bằng 0. vì như vậy là làm tròn. sẽ có sai số

		if (_canFlashes)
		{
			if (_spriteSheet->getOpacity() != _valueFlashes)
				_spriteSheet->setOpacity(_valueFlashes);
			else
				_spriteSheet->setOpacity(1.0f);
		}
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
bool Animation::isAnimate()
{
	// 7ung => use in AirCraft::draw
	return _canAnimate;
}
void Animation::addFrameRect(RECT rect)
{
	//nếu là rect đầu tiên thì set current luôn
	if (_frameRectList.empty())
	{
		_currentRect = rect;

		// trường hợp kiểm tra _bounding trước khi vẽ lần đầu tiên, nếu ko có setFrameRect thì nó sẽ lấy nguyên spriteSheet
		// sẽ làm sai kích thước của frame hiện tại
		// vậy lần đầu gán cho frame đầu. (nhưng bị lỗi ở AirCraft)
		// cách khác là setframeRect ở object cho sprite.

		// _spriteSheet->setFrameRect(_currentRect);
	}
		

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

void Animation::addFrameRect(eID id, char* firstRectName, ...)
{
	va_list vl;
	char* name;

	va_start(vl, firstRectName);

	name = firstRectName;

	while (name != NULL)
	{
		this->addFrameRect(SpriteManager::getInstance()->getSourceRect(id, name));
		name = va_arg(vl, char*);
	}

	va_end(vl);
}

void Animation::setLoop(bool isLoop)
{
	if (isLoop != _isLoop)
		_isLoop = isLoop;
}

bool Animation::isLoop()
{
	return _isLoop;
}

void Animation::restart()
{
	setIndex(0);

	if (_canAnimate == false)
		_canAnimate = true;
}

void Animation::enableFlashes(bool enable)
{
	if (_canFlashes == enable)
		return;

	_canFlashes = enable;
	_spriteSheet->setOpacity(1.0f);
}

void Animation::setValueFlashes(float value)
{
	if (_valueFlashes != value)
		_valueFlashes = value;
}

void Animation::draw(LPD3DXSPRITE spriteHandle, Viewport * viewport)
{
	_spriteSheet->setFrameRect(_currentRect);
	_spriteSheet->render(spriteHandle, viewport);
}
