#include "LifeUI.h"

LifeUI::LifeUI(GVector2 position, int number) : EmptyObject(eID::LIFE_ICON, position, 0, 0)
{
	for (int i = 0; i < number; i++)
	{
		auto sprite = SpriteManager::getInstance()->getSprite(eID::LIFE_ICON);
		sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "blue"));
		sprite->setScale(SCALE_FACTOR);
		_listIcons.push_back(sprite);
		_listIcons.back()->setPosition(position.x + (_listIcons.back()->getFrameWidth() + GAP) * i, position.y);
	}
}

LifeUI::~LifeUI()
{
}

void LifeUI::init()
{
	this->setScale(SCALE_FACTOR);
}

void LifeUI::update(float deltatime)
{
}

void LifeUI::draw(LPD3DXSPRITE spriteHandle, Viewport * viewport)
{
	// vẽ 3 cái ra thôi // 7ung: sửa thành 5 nhé :v
	if (_listIcons.size() > 5)
	{
		for (int i = 0; i < 5; i++)
			_listIcons[i]->render(spriteHandle);
	}
	else
	{
		for (auto icon : _listIcons)
		{
			icon->render(spriteHandle);
		}
	}

	//for (auto icon : _listIcons)
	//{
	//	icon->render(spriteHandle);
	//}
}

void LifeUI::release()
{
	while (!_listIcons.empty())
	{
		SAFE_DELETE(_listIcons.back());
		_listIcons.pop_back();
	}
}

void LifeUI::setLifeNumber(int number)
{
	if (_listIcons.size() == number || number < 0)
		return;

	if (_listIcons.size() < number)
	{
		while (_listIcons.size() < number)
		{
			auto sprite = SpriteManager::getInstance()->getSprite(eID::LIFE_ICON);
			sprite->setFrameRect(SpriteManager::getInstance()->getSourceRect(eID::LIFE_ICON, "blue"));
			sprite->setScale(SCALE_FACTOR);
			_listIcons.push_back(sprite);
			_listIcons.back()->setPosition(this->getPositionX() + (_listIcons.back()->getFrameWidth() + GAP) * (_listIcons.size() - 1), this->getPositionY());
		}
	}
	else
	{
		while (_listIcons.size() > number)
		{
			SAFE_DELETE(_listIcons.back());
			_listIcons.pop_back();
		}
	}
}

int LifeUI::getLifeNumber()
{
	return _listIcons.size();
}
