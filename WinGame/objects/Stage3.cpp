#include "Stage3.h"
#include "..\Tiles\ObjectFactory.h"
#include "BeginPlayScene.h"
#include "GameOverScene.h"
#include "Score.h"
#include "ShadowBeast.h"

#if _DEBUG
#include <time.h>
#endif

#include "IntroScene.h"
 
Stage3::Stage3(int billlife)
{
	_restBill = billlife;
	_viewport = new Viewport(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
} 

Stage3::~Stage3()
{
	delete _viewport;
	_viewport = nullptr;
}
void Stage3::setViewport(Viewport * viewport)
{
	if (_viewport != viewport)
		_viewport = viewport;
}

bool Stage3::init()
{
	auto bill = new Bill(_restBill);
	bill->init();
	bill->setPosition(200, 100);
	
	this->_bill = bill;
	_listControlObject.push_back(bill);
	_listobject.push_back(bill);

	auto bulletmanager = new BulletManager();
	bulletmanager->init();
	_listobject.push_back(bulletmanager);

	//auto shadow = new ShadowBeast(GVector2(256, 4124));
	//shadow->init();
	//_listobject.push_back(shadow);
	_text = new Text(L"Arial", "", 10, 25);

	map<string, BaseObject*>* maptemp = ObjectFactory::getMapObjectFromFile("Resources//Map//stage3.xml");

	this->_mapobject.insert(maptemp->begin(), maptemp->end());

	_root = QNode::loadQuadTree("Resources//Map//stage3_quadtree.xml");
	OutputDebugString(L"_root Success\n");

	background = Map::LoadFromFile("Resources//Map//stage3.xml",eID::MAPSTAGE3);
	OutputDebugString(L"background Success\n");

	SoundManager::getInstance()->PlayLoop(eSoundId::BACKGROUND_STAGE2);

	auto scenarioBoss_Viewport = new Scenario("BossViewport");
	__hook(&Scenario::update, scenarioBoss_Viewport, &Stage3::bossScene_Viewport);
	auto scenarioBossSound = new Scenario("BossSound");
	__hook(&Scenario::update, scenarioBossSound, &Stage3::playBossStage1Sound);
	_director = new ScenarioManager();
	_director->insertScenario(scenarioBossSound);
	_director->insertScenario(scenarioBoss_Viewport);

	auto scenarioKillBoss = new Scenario("KillBoss");
	__hook(&Scenario::update, scenarioKillBoss, &Stage3::killbossScene_Bill);
	auto playsound = new Scenario("PassBossSound");
	__hook(&Scenario::update, playsound, &Stage3::playPassBossSound);

	flagbossScenario = false;
	_directorKillBoss = new ScenarioManager();
	_directorKillBoss->insertScenario(playsound);
	_directorKillBoss->insertScenario(scenarioKillBoss);

	_flagCredit = false;
	_credit = new Credit();
	_credit->init();
	return true;
}
void Stage3::bossScene_Viewport(float dt, bool& finish)
{
	GVector2 current_position = _viewport->getPositionWorld();
	GVector2 worldsize = this->background->getWorldSize();

	current_position.y += BILL_MOVE_SPEED * dt / 1000;
	if (current_position.y > worldsize.y)
	{ 
		current_position.y = worldsize.y;
		finish = true;
		_viewport->setPositionWorld(current_position);
		return;
	}
	_viewport->setPositionWorld(current_position);
	//if (_bill->getBounding().left < current_position.x)
	//{
	//	GVector2 curPos = _bill->getPosition();
	//	curPos.x = current_position.x + (_bill->getSprite()->getFrameWidth() >> 1);
	//	_bill->setPosition(curPos);
	//}
	finish = false;
}
void Stage3::playBossStage1Sound(float dt, bool& finish)
{
	SoundManager::getInstance()->Play(eSoundId::BOSS_SOUNG1);
	finish = true;
}
void Stage3::playPassBossSound(float dt, bool& finish)
{
	SoundManager::getInstance()->Play(eSoundId::WINGAME);
	_flagCredit = true;

	((Bill*)_bill)->unhookinputevent();
	finish = true;
}

void Stage3::killbossScene_Bill(float deltatime, bool& isFinish)
{
	if (SoundManager::getInstance()->IsPlaying(eSoundId::WINGAME) == false)
	{
		isFinish = true;
	}
}

void Stage3::updateInput(float dt)
{
	for (IControlable* obj : _listControlObject)
	{
		obj->updateInput(dt);
	}

}

void Stage3::update(float dt)
{
	
	char str[100];
	sprintf(str, "delta time: %f", dt);
	_text->setText(str);

	// id của đối tượng, được get trong vòng lặp duyệt đối tượng.
	eID objectID;
	if (this->checkGameLife() == true)
		return;
	if (_bill->isInStatus(eStatus::DYING) == false)
	{
		this->updateViewport(_bill);
	}

	GVector2 viewport_position = _viewport->getPositionWorld();
	RECT viewport_in_transform = _viewport->getBounding();
	auto mapsize = this->background->getWorldSize();
	// Hàm getlistobject của quadtree yêu cầu truyền vào một hình chữ nhật theo hệ top left, nên cần tính lại khung màn hình
	RECT screen;
	// left right không đổi dù hệ top-left hay hệ bot-left
	screen.left = viewport_in_transform.left;
	screen.right = viewport_in_transform.right;
	screen.top = mapsize.y - viewport_position.y;
	screen.bottom = screen.top + _viewport->getHeight();

	// getlistobject
#if _DEBUG
	// clock_t để test thời gian chạy đoạn code update (milisecond)
	clock_t t;
	t = clock();
#endif

	// [Bước 1]
	this->destroyobject();

	// [Bước 2]
	_active_object.clear();

	// [Bước 3]
	auto listobjectname = _root->GetActiveObject(screen);

	// [Bước 4]
	for (auto name : listobjectname)
	{
		auto obj = _mapobject.find(name);
		if (obj == _mapobject.end() || obj._Ptr == nullptr)
			continue;
		_active_object.push_back(obj->second);
	}

	// [Bước 5]
	_active_object.insert(_active_object.end(), _listobject.begin(), _listobject.end());
	
	// [Bước 6]
	for (BaseObject* obj : _active_object)
	{
		for (BaseObject* passiveobj : _active_object)
		{
			obj->checkCollision(passiveobj, dt);
		}
	}

	// [Bước 7]
	for (BaseObject* obj : _active_object)
	{
		obj->update(dt);
	}

#if _DEBUG
	t = clock() - t;
	__debugoutput((float)t / CLOCKS_PER_SEC);
#endif


	this->ScenarioMoveViewport(dt);
	this->ScenarioKillBoss(dt);
}

void Stage3::destroyobject()
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
	for (auto name : QNode::ActiveObject)
	{
		auto object = _mapobject.find(name);
		if (object == _mapobject.end() || object._Ptr == nullptr)
			continue;
		if (object->second->getStatus() == eStatus::DESTROY)	// kiểm tra nếu là destroy thì loại khỏi list
		{
			if (dynamic_cast<BaseEnemy*> (object->second) != nullptr)
			{
				SoundManager::getInstance()->Play(eSoundId::DESTROY_ENEMY);
			}
			object->second->release();
			delete object->second;
			object->second = NULL;
			_mapobject.erase(name);
			
		}
	}
}

void Stage3::updateViewport(BaseObject* objTracker)
{
	// Vị trí hiện tại của viewport. 
	GVector2 current_position = _viewport->getPositionWorld();
	GVector2 worldsize = this->background->getWorldSize();
	GVector2 new_position = GVector2(0, max(objTracker->getPositionY() + 192, WINDOW_HEIGHT));		// 192 khoảng cách tối đa giữa object và map -> hardcode

	if (new_position.y < current_position.y)
	{
		new_position.y = current_position.y;
	}
	if (new_position.y > worldsize.y)
	{
		new_position.y = worldsize.y ;
	}


	_viewport->setPositionWorld(new_position);

	float billPositionX = objTracker->getPositionX();
	auto halfwidth = objTracker->getSprite()->getFrameWidth() * objTracker->getSprite()->getOrigin().x;
	if (billPositionX + halfwidth >= WINDOW_WIDTH)
	{
		objTracker->setPositionX(current_position.x + WINDOW_WIDTH - halfwidth);
	}
}

void Stage3::draw(LPD3DXSPRITE spriteHandle)
{
	//sprite->render(spriteHandle, _viewport);
	background->draw(spriteHandle, _viewport);

	for (BaseObject* object : _active_object)
	{
		object->draw(spriteHandle, _viewport);
	}
	if (_flagCredit == true)
	{
		_credit->draw(spriteHandle);
	}

#if _DEBUG
	_text->draw();
#endif
}

void Stage3::release()
{
	for (auto object : _listobject)
	{
		object->release();
		SAFE_DELETE(object);
	}
	background->release();
	SAFE_DELETE(background);
	SAFE_DELETE(_director);
	SAFE_DELETE(_directorKillBoss);
	SAFE_DELETE(_root);

}

void Stage3::ScenarioMoveViewport(float deltatime)
{
	if (_director == nullptr)
		return;
	
	if (_viewport->getPositionWorld().y > 4192.0f)
	{
		flagbossScenario = true;
	}
	if (flagbossScenario == true)
	{
		this->_director->update(deltatime);
		if (this->_director->isFinish() == true)
		{
			SAFE_DELETE(_director);
		}
	}
}
void Stage3::ScenarioKillBoss(float deltatime)
{
	if (_directorKillBoss == nullptr)
		return;
	auto boss = getObject(eID::SHADOW_BEAST);
	if ((SoundManager::getInstance()->IsPlaying(eSoundId::DESTROY_BOSS) == false) && boss != nullptr && boss->isInStatus(eStatus::DYING) == true)
	{
		_credit->update(deltatime);
		this->_directorKillBoss->update(deltatime);
		if (this->_directorKillBoss->isFinish() == true)
		{
			SAFE_DELETE(_directorKillBoss);
			//chuyển scene
			// test
			auto intro = new IntroScene();
			SceneManager::getInstance()->replaceScene(intro);
		}
		if (InputController::getInstance()->isKeyDown(DIK_Q))
		{
			auto intro = new IntroScene();
			if (SoundManager::getInstance()->IsPlaying(eSoundId::WINGAME))
			{
				SoundManager::getInstance()->Stop(eSoundId::WINGAME);
			}
			SceneManager::getInstance()->replaceScene(intro);
		}
	}
}
bool Stage3::checkGameLife()
{
	if (((Bill*)_bill)->getLifeNumber() < 0)
	{
		auto gameoverScene = new GameOverScene(Score::getScore(), 3);		// hardcode test: 1000 = số điểm
		SoundManager::getInstance()->Stop(eSoundId::BACKGROUND_STAGE2);
		SceneManager::getInstance()->replaceScene(gameoverScene);
		return true;
	}
	return false;
}
BaseObject* Stage3::getObject(eID id)
{
	if (id == eID::BILL)
		return getBill();
	eID objectID;
	if (_active_object.size() == 0)
	{
		return nullptr;
	}
	for (BaseObject* object : _active_object)
	{
		objectID = object->getId();
		if (objectID == id)
			return object;
	}
	return nullptr;
}

Bill* Stage3::getBill()
{
	return (Bill*)this->_bill;
}