#include "PlayScene.h"
#include "..\Tiles\ObjectFactory.h"

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
	bill->setPosition(400, 300);
	
	this->_bill = bill;
	_listControlObject.push_back(bill);
	_listobject.push_back(bill);

	auto bridge = new Bridge(GVector2(1552, 240));
	bridge->init();
	bridge->setPhysicsBodySide(eDirection::TOP);
	_listobject.push_back(bridge);

	auto soldier = new Soldier(RUNNING, 500, 400, -1);
	soldier->init();
	soldier->setStatus(eStatus::JUMPING);
	_listobject.push_back(soldier);

	_text = new Text(L"Arial", "", 10, 25);

	//auto aircraft = new AirCraft(START_POSITION, HORIZONTAL_VELOC, AIRCRAFT_AMPLITUDE, AIRCRAFT_FREQUENCY, eAirCraftType::I);
	//aircraft->init();
	//_listobject.push_back(aircraft);

	//auto rifleman = new Rifleman(NORMAL, 500, 200);
	//rifleman->init();
	//_listobject.push_back(rifleman);

	//auto redcannon = new RedCannon(NORMAL, 900, 200);
	//redcannon->init();
	//_listobject.push_back(redcannon);

	//auto wall_turret = new WallTurret(NORMAL, 730, 150);
	//wall_turret->init();
	//_listobject.push_back(wall_turret);

	//auto land  = new Land(64, 245, 1472, 1, eDirection::TOP, eLandType::GRASS);
	//land->init();
	//_listobject.push_back(land);

	//auto land2 = new Land(288 * 2, 48, 64 * 2, 5, eDirection::TOP, eLandType::GRASS);
	//land2->init();
	//land2->enableJump(false);
	//_listobject.push_back(land2);

	//auto land3 = new Land(64 * 5, 64 * 2 + 48, 64 * 3, 5, eDirection::TOP, eLandType::GRASS);
	//land3->init();
	//_listobject.push_back(land3);

	//auto land4 = new Land(64 * 8, 64 + 48, 64, 5, eDirection::TOP, eLandType::GRASS);
	//land4->init();
	//_listobject.push_back(land4);

	//auto water = new Land(0, 32, 288 * 2, 5, eDirection::TOP, eLandType::WATER);
	//water->init();
	//_listobject.push_back(water);

	//auto water2 = new Land(32 * 2 * 11, 32, 32 * 8 * 2, 5, eDirection::TOP, eLandType::WATER);
	//water2->init();
	//_listobject.push_back(water2);

	vector<BaseObject*>* temp = ObjectFactory::getListObjectFromFile("Resources//Map//stage1.xml");
	map<string, BaseObject*>* maptemp = ObjectFactory::getMapObjectFromFile("Resources//Map//stage1.xml");

	this->_listobject.insert(_listobject.end(), temp->begin(), temp->end());
	this->_mapobject.insert(maptemp->begin(), maptemp->end());

	_root = QNode::loadQuadTree("Resources//Map//stage1_quadtree.xml", this->_mapobject);

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

	this->updateViewport(_bill);
	
	//_listobject[0]->checkCollision(_listobject[1], dt);
	//_listobject[0]->checkCollision(_listobject[2], dt);
	//_listobject[0]->checkCollision(_listobject[3], dt);
	//_listobject[0]->checkCollision(_listobject[5], dt);
	//_listobject[0]->checkCollision(_listobject[7], dt);

	for (int i = 1; i < _listobject.size(); i++)
	{
		// bill check
		this->_bill->checkCollision(_listobject[i], dt);
	}
	
	// sodier
	for (int i = 1; i < _listobject.size(); i++)
	{
		_listobject[2]->checkCollision(_listobject[i], dt);
	}

	for (int i = 1; i < _listobject.size(); i++)
	{
		//_listobject[4]->checkCollision(_listobject[i], dt);

		// rifle man
		_listobject[46]->checkCollision(_listobject[i], dt);
		_listobject[47]->checkCollision(_listobject[i], dt);
		_listobject[53]->checkCollision(_listobject[i], dt);
		_listobject[58]->checkCollision(_listobject[i], dt);

		// aircraft
		_listobject[59]->checkCollision(_listobject[i], dt);
		_listobject[60]->checkCollision(_listobject[i], dt);
	}

	//_listobject[3]->checkCollision(_listobject[5], dt);


	for (auto object : _listobject)
	{
		if (isContains(this->getViewport()->getBounding(), object->getBounding()))
		{
			object->update(dt);
		}
	}
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

void PlayScene::updateViewport(BaseObject* objTracker)
{
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();
	GVector2 worldsize = this->background->getWorldSize();
	// Bám theo object.
	GVector2 new_position = GVector2(max(objTracker->getPositionX() - 200, 0), WINDOW_HEIGHT);

#if(!_DEBUG)
	// Không cho đi ngược
	if (new_position.x < current_position.x)
	{
		new_position.x = current_position.x;
	}
#endif

	// Không cho đi quá map.
	if (new_position.x + WINDOW_WIDTH > worldsize.x)
	{
		new_position.x = worldsize.x - WINDOW_WIDTH;
	}

	_viewport->setPositionWorld(new_position);
}

void PlayScene::draw(LPD3DXSPRITE spriteHandle)
{
	//sprite->render(spriteHandle, _viewport);
	background->draw(spriteHandle, _viewport);

	for (auto object : _listobject)
	{
		if (isContains(this->getViewport()->getBounding(), object->getBounding()))
		{
			object->draw(spriteHandle, _viewport);
		}
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