#include "PlayScene.h"


//Viewport* PlayScene::_viewport = new Viewport(0, WINDOW_HEIGHT);

PlayScene::PlayScene()
{
	_viewport = new Viewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);

}

PlayScene::~PlayScene()
{
	delete _viewport;
	_viewport = nullptr;
}
void PlayScene::setViewport(Viewport * viewport)
{
	if (_viewport != viewport)
		_viewport = viewport;
}

//Viewport * PlayScene::getViewport()
//{
//	return _viewport;
//}

bool PlayScene::init()
{
    
	//sprite = SpriteManager::getInstance()->getSprite(eID::MAPSTAGE1);
	//sprite->setIndex(4);
	//sprite->setPosition(50, 50);
	//
	//auto soldier = new Soldier();
	//soldier->init();
	//_listobject.push_back(soldier);

	//auto falcon = new Falcon();
	//falcon->init();
	//_listobject.push_back(falcon);

	//auto mario = new Mario();
	//mario->init();
	//_listobject.push_back(mario);
	//_listControlObject.push_back(mario);

	//auto redcannon = new RedCannon(GVector2(500, 500));
	//_listobject.push_back(redcannon);

	auto bill = new Bill();
	bill->init();
	bill->setPosition(400, 500);
	
	this->_bill = bill;
	_listControlObject.push_back(bill);
	_listobject.push_back(bill);


	auto box1 = new MyBox(0);
	box1->init();
	box1->setPosition(400, 180);
	_listobject.push_back(box1);

	auto bridge = new Bridge(GVector2(1552, 240));
	bridge->init();
	bridge->setPhysicsBodySide(eDirection::TOP);
	_listobject.push_back(bridge);

	auto box2 = new MyBox(1);
	box2->init();
	box2->setPosition(500, 180);
	_listobject.push_back(box2);

	auto soldier = new Soldier();
	soldier->init();
	soldier->setStatus(eStatus::JUMPING);
	_listobject.push_back(soldier);

	_text = new Text(L"Arial", "", 10, 25);

	auto aircraft = new AirCraft(START_POSITION, HORIZONTAL_VELOC, AIRCRAFT_AMPLITUDE, AIRCRAFT_FREQUENCY, eAirCraftType::I);
	aircraft->init();
	_listobject.push_back(aircraft);
	_listControlObject.push_back(aircraft);

	auto rifleman = new Rifleman();
	rifleman->init();
	_listobject.push_back(rifleman);

	auto grass = new Grass(GVector2(96.0f, 238.0f));
	grass->init();
	_listobject.push_back(grass);

	background = Map::LoadFromFile("Resources//Map//stage1.xml",eID::MAPSTAGE1);
	return true;
}

void PlayScene::updateInput(float dt)
{
	for (IControlable* obj : _listControlObject)
	{
		obj->updateInput(dt);
	}
}

void PlayScene::update(float dt)
{
	
	//test sprite
	//sprite->setPositionX(sprite->getPosition().x + 1);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 1);
	//sprite->setScale(2);
	//sprite->nextFrame();
	//sprite->setRotate(sprite->getRotate() + 10);

	char str[100];
	sprintf(str, "delta time: %f", dt);
	_text->setText(str);

	// id của đối tượng, được get trong vòng lặp duyệt đối tượng.
	eID objectID;

	for (auto object : _listobject)
	{
		object->update(dt);
	}
	_viewport->setPositionWorld(
		GVector2(max(_listobject[0]->getPositionX() - 200, 0), WINDOW_HEIGHT));
	
	_listobject[0]->checkCollision(_listobject[1], dt);
	_listobject[0]->checkCollision(_listobject[2], dt);
	_listobject[0]->checkCollision(_listobject[3], dt);
	_listobject[0]->checkCollision(_listobject[5], dt);
	_listobject[0]->checkCollision(_listobject[7], dt);

	_listobject[4]->checkCollision(_listobject[1], dt);
	_listobject[4]->checkCollision(_listobject[2], dt);
	_listobject[4]->checkCollision(_listobject[3], dt);

	//_listobject[5]->checkCollision(_listobject[0], dt);
	_listobject[5]->checkCollision(_listobject[1], dt);

}

void PlayScene::destroyobject()
{
	for (auto object : _listobject)
	{
		if (object->getStatus() == eStatus::DESTROY)	// kiểm tra nếu là destroy thì loại khỏi list
		{
			object->release();
			// http://www.cplusplus.com/reference/algorithm/remove/
			auto rs1 = std::remove(_listobject.begin(), _listobject.end(), object);
			_listobject.pop_back();			// sau khi remove thì còn một phần tử cuối cùng vôi ra. giống như dịch mảng. nên cần bỏ nó đi

			//https://msdn.microsoft.com/en-us/library/cby9kycs.aspx (dynamic_cast) 
			// loại khỏi list control
			vector<IControlable*>::iterator icontrol = find(_listControlObject.begin(), _listControlObject.end(), dynamic_cast<IControlable*>(object));
			if (icontrol != _listControlObject.end())
			{
				auto rs2 = std::remove(_listControlObject.begin(), _listControlObject.end(), (*icontrol));
				_listControlObject.pop_back();
			}
			delete object;
			break;		// sau pop_back phần tử đi thì list bị thay đồi, nên vòng for-each không còn nguyên trạng nữa. -> break (mỗi frame chỉ remove được 1 đối tượng)
		}
	}
}
void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	//sprite->render(spriteHandle, _viewport);
	background->draw(spriteHandle, _viewport);
	for (auto object : _listobject)
	{
		object->draw(spriteHandle, _viewport);
	}
#if _DEBUG
	_text->draw();
#endif
}

void PlayScene::release()
{
	for (auto object : _listobject)
	{
		object->release();
	}
}

BaseObject* PlayScene::getObject(eID id)
{
	if (id == eID::BILL)
		return getBill();
	eID objectID;
	for (BaseObject* object : _listobject)
	{
		objectID = object->getId();
		if (objectID == id)
			return object;
	}
	return nullptr;
}

Bill* PlayScene::getBill()
{
	return (Bill*)this->_bill;
}