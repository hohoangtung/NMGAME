
#include "Bridge.h"

int Bridge::_matrixIndex[2][MAX_WAVE * 2] =
	{
		{ 3, 0, 3, 0, 3, 0, 3, 0 },
		{ 1, 4, 4, 4, 4, 4, 4, 5 },

	};
Bridge::Bridge() : BaseObject(eID::BRIDGE)
{
	_sprite = SpriteManager::getInstance()->getSprite(eID::BRIDGE);
}
void Bridge::init()
{
	__hook(&InputController::__eventkeyReleased, _input, &Bridge::testExplose);
	_stopwatch = new StopWatch();
	_transform = new Transformable();
	_transform->setPosition(200, 200);
	_explode = new QuadExplose(_transform->getPosition());
	_explode->init();
	_wave = 0;
}
void Bridge::update(float deltatime)
{
	if (this->getStatus() == eStatus::BURST)
		this->burst(deltatime);
}
void Bridge::updateInput(float deltatime)
{

}
void Bridge::burst(float deltatime)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	_matrixIndex[0][0 + _wave * 2] =
		_matrixIndex[0][1 + _wave * 2] =
		_matrixIndex[1][0 + _wave * 2] =
		_matrixIndex[1][1 + _wave * 2] = -1;
	if (_explode->getStatus() == NORMAL)
		_explode->update(deltatime);
	if (_explode->getStatus() == DESTROY)
	{
		_explode->release();

		if (_stopwatch->isStopWatch(DELAYTIME))
		{
			_wave++;
			GVector2 pos = this->getPosition();
			pos.x += 32 * _wave;			// HARD CODE
			_explode = new QuadExplose(pos);
			_explode->init();
			_stopwatch->restart();
			if (_wave == MAX_WAVE)
			{
				_explode->setStatus(eStatus::DESTROY);
				this->setStatus(eStatus::DESTROY);
			}
		}
	}
}
void Bridge::testExplose(KeyEventArg* key)
{
	if (key == NULL)
		return;
	if (key->_key == DIK_E)
		this->setStatus(eStatus::BURST);
}
void Bridge::draw(LPD3DXSPRITE spritehandle, Viewport* viewport)
{
	if (this->getStatus() == eStatus::DESTROY)
		return;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < MAX_WAVE * 2; j++)
		{
			if (_matrixIndex[i][j] == -1)
				continue;
			auto pos = this->_transform->getPosition();
			pos.x += j * 16;							// HARD CODE
			pos.y -= i * 16;
			_sprite->setPosition(pos);
			_sprite->setIndex(_matrixIndex[i][j]);
			_sprite->render(spritehandle, viewport);
		}
	}
	if (this->getStatus() == eStatus::BURST)
	{
		_explode->draw(spritehandle, viewport);
	}
}
void Bridge::release()
{

}
void Bridge::setPosition(GVector2 position)
{
	this->_transform->setPosition(position);
}
GVector2 Bridge::getPosition()
{
	return	this->_transform->getPosition();
}

Bridge::~Bridge()
{
}
Bridge::QuadExplose::QuadExplose(GVector2 position) : BaseObject(eID::QUADEXPLODE)
{
	_transform = new Transformable();
	this->_transform->setPosition(position);
}
void Bridge::QuadExplose::init()
{	
	_timer = 0.0f;
	auto pos = _transform->getPosition();

	_explosion1 = new Explosion(2);
	_explosion1->init();
	_explosion1->setPosition(pos);

	_explosion2 = new Explosion(2);
	_explosion2->init();
	_explosion2->setPosition(GVector2(pos.x + 16, pos.y - 16));

	_explosion3 = new Explosion(2);
	_explosion3->init();
	_explosion3->setPosition(GVector2(pos.x, pos.y - 16));

	_explosion4 = new Explosion(2);
	_explosion4->init();
	_explosion4->setPosition(GVector2(pos.x - 16, pos.y - 16));
}

void Bridge::QuadExplose::update(float deltatime)
{
	_timer += deltatime;
	if (_timer >= 0)
		_explosion1->update(deltatime);
	if (_timer >= 50)
		_explosion2->update(deltatime);
	if (_timer >= 100)
		_explosion3->update(deltatime);
	if (_timer >= 150)
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
	if (_timer >= 50)
		_explosion2->draw(spritehandle, viewport);
	if (_timer >= 100)
		_explosion3->draw(spritehandle, viewport);
	if (_timer >= 150)
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

}