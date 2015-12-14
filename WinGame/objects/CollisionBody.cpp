#include "CollisionBody.h"

CollisionBody::CollisionBody(BaseObject * target)
{
	_target = target;

	_collisionBodyRect.top = 0;
	_collisionBodyRect.left = 0;
	_collisionBodyRect.right = abs(_target->getBounding().left - _target->getBounding().right);
	_collisionBodyRect.left = abs(_target->getBounding().top - _target->getBounding().bottom);
}

CollisionBody::CollisionBody(BaseObject * target, RECT bodyRect)
{
	_target = target;
	_collisionBodyRect = bodyRect;
}

CollisionBody::~CollisionBody()
{

}

void CollisionBody::checkCollision(BaseObject * otherObject, float dt, bool updatePosition)
{
	eDirection direction;
	float time = isCollide(otherObject, direction, dt);

	if (time < 1.0f)
	{
		if (otherObject->getPhysicsBodySide() != eDirection::NONE && (direction & otherObject->getPhysicsBodySide()) == direction)
		{
			// cập nhật tọa độ
			updateTargetPosition(otherObject, direction, true);
		}

		CollisionEventArg* e = new CollisionEventArg(otherObject);
		e->_sideCollision = direction;

		__raise onCollisionBegin(e);
		
		_listColliding[otherObject] = true;
	}
	else if(_listColliding.find(otherObject) == _listColliding.end())	// ko có trong list đã va chạm
	{
		if (isColliding(_target->getBounding(), otherObject->getBounding()))
		{
			CollisionEventArg* e = new CollisionEventArg(otherObject);
			e->_sideCollision = this->getSide(otherObject);

			__raise onCollisionBegin(e);

			_listColliding[otherObject] = true;
		}
	}
	else	// có trong list đã va chạm
	{
		float moveX, moveY;
		if (isColliding(otherObject, moveX, moveY, dt))		// kt va trạm lấy khoảng chồng lấp của 2 object
		{
			auto side = this->getSide(otherObject);

			if (otherObject->getPhysicsBodySide() == eDirection::NONE || (side & otherObject->getPhysicsBodySide()) != side)
				return;

			// cập nhật tọa độ
			if (updatePosition)
				updateTargetPosition(otherObject, side, false, GVector2(moveX, moveY));
		}
		else // nếu ko va chạm nữa là kết thúc va chạm
		{
			CollisionEventArg* e = new CollisionEventArg(otherObject);
			e->_sideCollision = eDirection::NONE;

			__raise onCollisionEnd(e);
			_listColliding.erase(otherObject);
		}
	}
	
}

bool CollisionBody::checkCollision(BaseObject * otherObject, eDirection & direction, float dt, bool updatePosition)
{
	float time = isCollide(otherObject, direction, dt);

	if (time < 1.0f)
	{
		if (otherObject->getPhysicsBodySide() != eDirection::NONE && (direction & otherObject->getPhysicsBodySide()) == direction)
		{
			// cập nhật tọa độ
			updateTargetPosition(otherObject, direction, true);
		}

		return true;
	}
	else
	{
		float moveX, moveY;
		if (isColliding(otherObject, moveX, moveY, dt))
		{
			auto side = this->getSide(otherObject);
			direction = side;

			if (otherObject->getPhysicsBodySide() == eDirection::NONE || (side & otherObject->getPhysicsBodySide()) != side)
				return true;

				// cập nhật tọa độ
				if (updatePosition)
					updateTargetPosition(otherObject, direction, false, GVector2(moveX, moveY));

				return true;
		}
	}

	direction = eDirection::NONE;

	return false;
}

float CollisionBody::isCollide(BaseObject * otherSprite, eDirection & direction, float dt)
{
	RECT myRect = _target->getBounding();
	RECT otherRect = otherSprite->getBounding();

	// sử dụng Broadphase rect để kt vùng tiếp theo có va chạm ko
	RECT broadphaseRect = getSweptBroadphaseRect(_target, dt);	// là bound của object được mở rộng ra thêm một phần bằng với vận tốc (dự đoán trước bound)
	if (!isColliding(broadphaseRect, otherRect))				// kiểm tra tính chồng lắp của 2 hcn
	{
		direction = eDirection::NONE;
		return 1.0f;
	}

	//SweptAABB
	// vận tốc mỗi frame
	GVector2 otherVeloc = GVector2(otherSprite->getVelocity().x * dt / 1000, otherSprite->getVelocity().y * dt / 1000);
	GVector2 myVelocity = GVector2(_target->getVelocity().x * dt / 1000, _target->getVelocity().y * dt / 1000);
	GVector2 velocity = myVelocity;

	if (otherVeloc != GVector2(0, 0))
	{
		velocity = otherVeloc - myVelocity;
	}

	// tìm khoảng cách giữa cạnh gần nhất, xa nhất 2 object dx, dy
	// dx
	if (velocity.x > 0)
	{
		_dxEntry = otherRect.left - myRect.right;
		_dxExit = otherRect.right - myRect.left;
	}
	else
	{
		_dxEntry = otherRect.right - myRect.left;
		_dxExit = otherRect.left - myRect.right;
	}
 
	// dy
	if (velocity.y > 0)
	{
		_dyEntry = otherRect.bottom - myRect.top;
		_dyExit = otherRect.top - myRect.bottom;
	}
	else
	{
		_dyEntry = otherRect.top - myRect.bottom;
		_dyExit = otherRect.bottom - myRect.top;
	}

	// tìm thời gian va chạm 
	if (velocity.x == 0)
	{
		// chia cho 0 ra vô cực
		_txEntry = -std::numeric_limits<float>::infinity();
		_txExit = std::numeric_limits<float>::infinity();
	}

	else
	{
		_txEntry = _dxEntry / velocity.x;
		_txExit = _dxExit / velocity.x;
	}

	if (velocity.y == 0)
	{
		_tyEntry = -std::numeric_limits<float>::infinity();
		_tyExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		_tyEntry = _dyEntry / velocity.y;
		_tyExit = _dyExit / velocity.y;
	}

	// thời gian va chạm
	// va chạm khi x và y CÙNG chạm => thời gian va chạm trễ nhất giữa x và y
	float entryTime = max(_txEntry, _tyEntry);
	// hết va chạm là 1 trong 2 x, y hết va chạm => thời gian sớm nhất để kết thúc va chạm
	float exitTime = min(_txExit, _tyExit);

	// object không va chạm khi:
	// nếu thời gian bắt đầu va chạm hơn thời gian kết thúc va chạm
	// thời gian va chạm x, y nhỏ hơn 0 (chạy qua luôn, 2 thằng đang đi xa ra nhau)
	// thời gian va chạm x, y lớn hơn 1 (còn xa quá chưa thể va chạm)
	if (entryTime > exitTime || _txEntry < 0.0f && _tyEntry < 0.0f || _txEntry > 1.0f || _tyEntry > 1.0f)
	{
		// không va chạm trả về 1 đi tiếp bt
		direction = eDirection::NONE;
		return 1.0f;
	}

	// nếu thời gian va chạm x lơn hơn y
	if (_txEntry > _tyEntry)
	{
		// xét x
		// khoảng cách gần nhất mà nhỏ hơn 0 nghĩa là thằng kia đang nằm bên trái object này => va chạm bên phải nó
		if (_dxEntry < 0.0f)
		//if(_dxExit < 0)
		{
			direction = eDirection::RIGHT;
		}
		else
		{
			direction = eDirection::LEFT;
		}
	}
	else
	{
		// xét y
		if (_dyEntry < 0.0f)
		//if(_dyExit < 0.0f)
		{
			direction = eDirection::TOP;
		}
		else
		{
			direction = eDirection::BOTTOM;
		}
	}

	return entryTime;
}

bool CollisionBody::isColliding(BaseObject * otherObject, float & moveX, float & moveY, float dt)
{
	moveX = moveY = 0.0f;
	auto myRect = _target->getBounding();
	auto otherRect = otherObject->getBounding();

	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	// kt coi có va chạm không
	//  CÓ va chạm khi 
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return false;

	// tính offset x, y để đi hết va chạm
	// lấy khoảng cách nhỏ nhất
	moveX = abs(left) < right ? left : right;
	moveY = top < abs(bottom) ? top : bottom;

	// chỉ lấy phần lấn vào nhỏ nhất
	if (abs(moveX) < abs(moveY))
		moveY = 0.0f;
	else
		moveX = 0.0f;

	return true;
}

void CollisionBody::updateTargetPosition(BaseObject* otherObject, eDirection direction, bool withVelocity, GVector2 move)
{
	if (withVelocity == true)
	{
		if (otherObject->getPhysicsBodySide() != eDirection::NONE || (direction & otherObject->getPhysicsBodySide()) == direction)
		{
			auto v = _target->getVelocity();
			auto pos = _target->getPosition();
			if (_txEntry > _tyEntry)
			{
				// xử lý cản left và right
				if (_txEntry < 1 && _txEntry > 0)
					pos.x += _dxEntry;
			}
			else
			{
				// xử lý cản top và bot
				if (_tyEntry < 1 && _tyEntry > 0)
					pos.y += _dyEntry;
			}
			_target->setPosition(pos);
		}
	}
	else
	{
		if (move.y > 0 && (otherObject->getPhysicsBodySide() & eDirection::TOP) == eDirection::TOP && _target->getVelocity().y <= 0)
		{
			_target->setPositionY(_target->getPositionY() + move.y);
		}
		else if (move.y < 0 && (otherObject->getPhysicsBodySide() & eDirection::BOTTOM) == eDirection::BOTTOM && _target->getVelocity().y >= 0)
		{
			_target->setPositionY(_target->getPositionY() + move.y);
		}

		if (move.x > 0 && (otherObject->getPhysicsBodySide() & eDirection::RIGHT) == eDirection::RIGHT && _target->getVelocity().x <= 0)
		{
			_target->setPositionX(_target->getPositionX() + move.x);
		}
		else if (move.x < 0 && (otherObject->getPhysicsBodySide() & eDirection::LEFT) == eDirection::LEFT && _target->getVelocity().x >= 0)
		{
			_target->setPositionX(_target->getPositionX() + move.x);
		}
	}
}

bool CollisionBody::isColliding(RECT myRect, RECT otherRect)
{
	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

RECT CollisionBody::getSweptBroadphaseRect(BaseObject* object, float dt)
{
	// vận tốc mỗi frame
	auto velocity = GVector2(object->getVelocity().x * dt / 1000, object->getVelocity().y * dt / 1000);
	auto myRect = object->getBounding();

	RECT rect;
	rect.top = velocity.y > 0 ? myRect.top + velocity.y : myRect.top;
	rect.bottom = velocity.y > 0 ? myRect.bottom : myRect.bottom + velocity.y;
	rect.left = velocity.x > 0 ? myRect.left : myRect.left + velocity.x;
	rect.right = velocity.y > 0 ? myRect.right + velocity.x : myRect.right;

	return rect;
}

eDirection CollisionBody::getSide(BaseObject* otherObject)
{
	auto myRect = _target->getBounding();
	auto otherRect = otherObject->getBounding();

	float left = otherRect.left - myRect.right;
	float top = otherRect.top - myRect.bottom;
	float right = otherRect.right - myRect.left;
	float bottom = otherRect.bottom - myRect.top;

	// kt va chạm
	if (left > 0 || right < 0 || top < 0 || bottom > 0)
		return eDirection::NONE;

	float minX;
	float minY;
	eDirection sideY;
	eDirection sideX;

	if (top > abs(bottom))
	{
		minY = bottom;
		sideY = eDirection::BOTTOM;
	}
	else
	{
		minY = top;
		sideY = eDirection::TOP;
	}
		
	
	if (abs(left) > right)
	{
		minX = right;
		sideX = eDirection::RIGHT;
	}
	else
	{
		minX = left;
		sideX = eDirection::LEFT;
	}


	if (abs(minX) < abs(minY))
	{
		return sideX;
	}
	else
	{
		return sideY;
	}
}

RECT CollisionBody::getCollisionRect()
{
	RECT rect;

	rect.top = _target->getBounding().top - _collisionBodyRect.top;
	rect.left = _target->getBounding().left + _collisionBodyRect.left;

	rect.bottom = rect.top - abs(_collisionBodyRect.top - _collisionBodyRect.bottom);
	rect.right = rect.left + abs(_collisionBodyRect.right - _collisionBodyRect.left);

	return rect;
}

bool CollisionBody::isColliding(BaseObject* otherObject)
{
	if (_listColliding.find(otherObject) != _listColliding.end())
		return true;
	else
		return false;
}

void CollisionBody::update(float dt)
{
}