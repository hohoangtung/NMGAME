
#include "Bridge.h"

int Bridge::_matrixIndex[2][MAX_WAVE * 2] =
{
	{ 3, 0, 3, 0, 3, 0, 3, 0 },
	{ 1, 4, 4, 4, 4, 4, 4, 5 },
};

#pragma region Bridge

Bridge::Bridge(GVector2 position) : Land((int)position.x, (int)position.y,0, 32,eDirection::TOP,eLandType::GRASS )
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BRIDGE);
	_sprite->setScale(SCALE_FACTOR);
	_transform = new Transformable();
	_transform->setPosition(position);

}

void Bridge::init()
{
	_stopwatch = new StopWatch();

	_explode = new QuadExplose(_transform->getPosition());
	_explode->init();
	_wave = 0;
	_stopwatch->restart();
	_listComponent["CollisionBody"] = new CollisionBody(this);

	for (int i = 0; i < 2; i++)
	{
		this->privateIndex[i] = new int[MAX_WAVE * 2];
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_WAVE * 2; j++)
		{
			this->privateIndex[i][j] = _matrixIndex[i][j];
		}
	}
}
void Bridge::update(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	if (this->getStatus() == eStatus::BURST)
	{
		this->burst(deltatime);
	}
	else
	{
		auto playscene = (PlayScene*) SceneManager::getInstance()->getCurrentScene();
		this->trackBill(playscene->getBill());
	}
	for (auto component : _listComponent)
	{
		component.second->update(deltatime);
	}
}

void Bridge::burst(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;

	privateIndex[0][0 + _wave * 2] =
		privateIndex[0][1 + _wave * 2] =
		privateIndex[1][0 + _wave * 2] =
		privateIndex[1][1 + _wave * 2] = -1;
	if (_explode->getStatus() == NORMAL)
		_explode->update(deltatime);
	if (_explode->getStatus() == DESTROY)
	{
		_explode->release();
		if (_stopwatch->isStopWatch(30))
		{
			_wave++;
			GVector2 pos = this->getPosition();
			pos.x += this->_sprite->getFrameWidth() * this->_sprite->getScale().x * _wave;
			_explode = new QuadExplose(pos);
			_explode->init();
			_stopwatch->restart();
			if (_wave == MAX_WAVE)
			{
				_explode->setStatus(eStatus::DESTROY);
				this->setStatus(eStatus::DESTROY);
			}
			else
			{
				SoundManager::getInstance()->Play(eSoundId::DESTROY_BRIDGE);
			}
		}
	}
}

void Bridge::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	GVector2 posrender;
	// Thuật toán vẽ giống cách vẽ từng pixel trên console.
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_WAVE * 2; j++)
		{
			if (privateIndex[i][j] == -1)
				continue;
			posrender = this->_transform->getPosition();
			posrender.x += j * this->getSprite()->getFrameWidth();
			posrender.y -= i * this->getSprite()->getFrameHeight();
			_sprite->setPosition(posrender);
			_sprite->setIndex(privateIndex[i][j]);
			_sprite->render(spritehandle, viewport);
		}
	}
	if (this->getStatus() == eStatus::BURST)
	{
		_explode->draw(spritehandle, viewport);
	}

	//RECT r;
	//auto pos = viewport->getPositionInViewport(new GVector3(getPositionX(), getPositionY(), 0));
	//r.top = max(pos.y, 1);
	//r.left = max(pos.x, 1);
	//r.bottom = min(pos.y + this->getBounding().top - this->getBounding().bottom, WINDOW_HEIGHT - 1);
	//r.right = min(pos.x + this->getBounding().right - this->getBounding().left, WINDOW_WIDTH - 1);

	//DeviceManager::getInstance()->getDevice()->ColorFill(_surface, NULL, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	//DeviceManager::getInstance()->getDevice()->StretchRect(
	//	_surface,
	//	NULL,
	//	DeviceManager::getInstance()->getSurface(),
	//	&r,
	//	D3DTEXF_NONE
	//	);
	//
}
void Bridge::release()
{
	SAFE_DELETE(_stopwatch);
	SAFE_DELETE(_transform);
	for (auto item : _listComponent)
	{
		SAFE_DELETE(item.second);
	}
	_listComponent.clear();
	SAFE_DELETE(_sprite);
}
void Bridge::setPosition(GVector2 position)
{
	this->_transform->setPosition(position);
}
GVector2 Bridge::getPosition()
{
	return	this->_transform->getPosition();
}
RECT Bridge::getBounding()
{
	RECT rect = {0,0,0,0};
	if (this->getStatus() == eStatus::DESTROY || _wave == MAX_WAVE - 1)
	{
		return rect;
	}

	int framewidth = this->_sprite->getFrameWidth(); // 32
	int frameheight = this->_sprite->getFrameHeight(); // 32
	rect.left = this->getPosition().x - (framewidth >> 2);					// framewidth /2 là origin(Anchor).
	rect.bottom = this->getPosition().y - frameheight - (frameheight >> 1);
	rect.right = rect.left + ((framewidth * MAX_WAVE) << 1);					// Nhân 2 vì cách 2 hình có 1 vụ nổ.
	rect.top = rect.bottom + frameheight + (frameheight >> 1) + 2;				// < 1 là nhân 2; > 1 là chia 2. Trừ đi frameheight / 2 đẻ bằng với land

	if (this->getStatus() == eStatus::BURST)
	{
		rect.left += (_wave + 1) * (framewidth << 1);
	}
	return rect;
}

void Bridge::trackBill(Bill* bill)
{
	RECT billBound = bill->getBounding();
	RECT bridgeBound = this->getBounding();

	if(billBound.right >= bridgeBound.left)
		this->setStatus(eStatus::BURST);
}

void Bridge::setStatus(eStatus status)
{
	this->_status = status;
	if (status == eStatus::BURST)
	{
		SoundManager::getInstance()->Play(eSoundId::DESTROY_BRIDGE);
	}
}
Bridge::~Bridge()
{
}

#pragma endregion

#pragma region QuadExplose

Bridge::QuadExplose::QuadExplose(GVector2 position) : BaseObject(eID::QUADEXPLODE)
{
	_transform = new Transformable();
	this->_transform->setPosition(position);
}
void Bridge::QuadExplose::init()
{	
	// Những số cứng 16 bên dưới là khoảng cách các vụ nổ nhỏ trong vụ nổ gồm 4 phát nổ.
	// Chỉ có mục đích thẩm mĩ, không có mục đích nào khác.
	_timer = 0.0f;
	auto pos = _transform->getPosition();
	_explosion1 = new Explosion(2);
	_explosion1->init();
	_explosion1->setScale(SCALE_FACTOR);
	_explosion1->setPosition(pos);
	((Explosion*)_explosion1)->setTimeAnimated(0.06f);

	_explosion2 = new Explosion(2);
	_explosion2->init();
	_explosion2->setScale(SCALE_FACTOR);
	_explosion2->setPosition(GVector2(pos.x + 16, pos.y - 16));
	((Explosion*)_explosion2)->setTimeAnimated(0.06f);

	_explosion3 = new Explosion(2);
	_explosion3->init();
	_explosion3->setScale(SCALE_FACTOR);
	_explosion3->setPosition(GVector2(pos.x, pos.y - 16));
	((Explosion*)_explosion3)->setTimeAnimated(0.06f);

	_explosion4 = new Explosion(2);
	_explosion4->init();
	_explosion4->setScale(SCALE_FACTOR);
	_explosion4->setPosition(GVector2(pos.x - 16, pos.y - 16));
	((Explosion*)_explosion4)->setTimeAnimated(0.06f);


}

void Bridge::QuadExplose::update(float deltatime)
{
	_timer += deltatime;
	if (_timer >= 0)
		_explosion1->update(deltatime);
	if (_timer >= 70)
		_explosion2->update(deltatime);
	if (_timer >= 140)
		_explosion3->update(deltatime);
	if (_timer >= 210)
	{
		_explosion4->update(deltatime);
		if (_explosion4->getStatus() == eStatus::DESTROY)
			this->setStatus(eStatus::DESTROY);
	}
}
void Bridge::QuadExplose::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->getStatus() != eStatus::NORMAL)
		return;
	if (_timer >= 0)
		_explosion1->draw(spritehandle, viewport);
	if (_timer >= 70)
		_explosion2->draw(spritehandle, viewport);
	if (_timer >= 140)
		_explosion3->draw(spritehandle, viewport);
	if (_timer >= 210)
		_explosion4->draw(spritehandle, viewport);
}
void Bridge::QuadExplose::setPosition(GVector2 position)
{
	this->_transform->setPosition(position);
	_explosion1->setPosition(position);
	_explosion2->setPosition(GVector2(position.x + 16, position.y - 16));
	_explosion3->setPosition(GVector2(position.x, position.y - 16));
	_explosion4->setPosition(GVector2(position.x - 16, position.y - 16));
}
GVector2 Bridge::QuadExplose::getPosition()
{
	return	this->_transform->getPosition();
}
void Bridge::QuadExplose::reset()
{
	_explosion1->setStatus(eStatus::NORMAL);
	_explosion2->setStatus(eStatus::NORMAL);
	_explosion3->setStatus(eStatus::NORMAL);
	_explosion4->setStatus(eStatus::NORMAL);
	this->setStatus(eStatus::NORMAL);
	_timer = 0.0f;
}
void Bridge::QuadExplose::release()
{
	if (_explosion1 != nullptr)
	{
		_explosion1->release();
		delete _explosion1;
		_explosion1 = nullptr;
	}
	if (_explosion2 != nullptr)
	{
		_explosion2->release();
		delete _explosion2;
		_explosion2 = nullptr;
	}
	if (_explosion3 != nullptr)
	{
		_explosion3->release();
		delete _explosion3;
		_explosion3 = nullptr;
	}
	if (_explosion4 != nullptr)
	{
		_explosion4->release();
		delete _explosion4;
		_explosion4 = nullptr;
	}
	SAFE_DELETE(_transform);

}

#pragma endregion