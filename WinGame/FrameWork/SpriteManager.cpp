/*
	HOW TO USE:
	spritemanager is basic class to manage load and release sprite and texture.
	It load all image you have, if you want to use sprite any where, just getSprite from SpriteManager.
	if you dont want you use this image any more, you can call release.

	everytime use this, you have to call SpriteManager::getInstance() first.
	this is Singleton format.
	Seemore about Singleton: http://www.stdio.vn/articles/read/224/singleton-pattern

	you can call directly: SpriteManager::getInstance()->DOSOMETHING()
	or reference it:
		 	SpriteManager* _spritemanager;
			...
			_spritemanager = SpriteManager::getInstance();
			...
			_spritemanager->DOSOMETHING()

	Next, you should call loadresource(LPD3DXSPRITE spriteHandle) in Game::loadresource().
	It will load all your image from file to memory.
	Remember: Insert your code to loadresource to load image from file.

	If you want object reference to Sprite. call:
		SpriteManager::getInstance()->getSprite(eID::OBJECT_ID)
		what is eID::OBJECT_ID ?  
		in define.h you can insert element to eID.

	OK. Now you have sprite to draw.

	If you dont want to use this sprite any more, call releaseSprite or releaseTexture
	they often are called in object::release
		
	Call SpriteManager::release() in game:release() to clean all memory.
*/
#include "SpriteManager.h"
US_FRAMEWORK

SpriteManager* SpriteManager::_instance = nullptr;
SpriteManager::SpriteManager(void)
{
	// do nothing.
}

SpriteManager::~SpriteManager(void)
{
	for (auto spr = _listSprite.begin(); spr != _listSprite.end(); ++spr)
	{
		spr->second->release();		// release image
		delete spr->second;			// delete sprite
	}
	if (_listSprite.empty() == false)
		_listSprite.clear();		// remove all from MAP
}

void SpriteManager::loadResource(LPD3DXSPRITE spriteHandle)
{
	/* if you have any image, load them with this format */
	// [psedue code]
	// sp = new SPRITE(...)
	// this->_listSprite.insert(pair<eID, Sprite*>(eID::ENUMOBJECT, sp));

	//Sprite* sp = new Sprite(spriteHandle, L"Flower.png", 4, 4);
	//this->_listSprite.insert(pair<eID, Sprite*>(eID::FLOWER, sp));

	//sp = new Sprite(spriteHandle, L"mario.png", 5, 5);
	//this->_listSprite.insert(pair<eID, Sprite*>(eID::MARIO, sp));

	Sprite* sp = NULL;
	sp = new Sprite(spriteHandle, L"Resources\\Soldier.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::SOLDIER, sp));
	this->loadSpriteInfo(eID::SOLDIER, "Resources\\soldier_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\Falcon.png");
	this->_listSprite.insert(pair<eID, Sprite*>(eID::FALCON, sp));
	this->loadSpriteInfo(eID::FALCON, "Resources\\falcon_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\Cannon_all.png", 15, 5);
	//this->_listSprite.insert(pair<eID, Sprite*>(eID::REDCANON, sp));
	this->_listSprite[eID::REDCANNON] = sp;
	this->loadSpriteInfo(eID::REDCANNON, "Resources\\cannon_animation.txt");


	sp = new Sprite(spriteHandle, L"Resources\\wall_turret_all.png", 42, 9);
	this->_listSprite[eID::WALL_TURRET] = sp;
	this->loadSpriteInfo(eID::WALL_TURRET, "Resources\\Wall_turret_animation.txt");

	

	sp = new Sprite(spriteHandle, L"Resources\\aircraft.png", 10, 10);
	this->_listSprite[eID::AIRCRAFT] = sp;
	this->loadSpriteInfo(eID::AIRCRAFT, "Resources\\aircraft_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\falcon.png", 7, 7);
	this->_listSprite[eID::FALCON] = sp;
	this->loadSpriteInfo(eID::FALCON, "Resources\\falcon_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\explosion.png");
	this->_listSprite[eID::EXPLOSION] = sp;
	this->loadSpriteInfo(eID::EXPLOSION, "Resources\\explosion_animation.txt");


	Sprite* bill = new Sprite(spriteHandle, L"Resources\\bill_animation.png");
	this->_listSprite[eID::BILL] = bill;
	this->loadSpriteInfo(eID::BILL, "Resources\\bill_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\rifleman.png");
	this->_listSprite[eID::RIFLEMAN] = sp;
	this->loadSpriteInfo(eID::RIFLEMAN, "Resources\\rifleman_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\boss_stage1.png");
	this->_listSprite[eID::BOSS_STAGE1] = sp;
	this->loadSpriteInfo(eID::BOSS_STAGE1, "Resources\\boss_stage1_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\bridge.png", 6, 3);
	this->_listSprite[eID::BRIDGE] = sp;

	auto bl = new Sprite(spriteHandle, L"Resources\\Bullets.png");
	this->_listSprite[eID::BULLET] = bl;
	this->loadSpriteInfo(eID::BULLET, "Resources\\bullets_type.txt");

	sp = new Sprite(spriteHandle, L"Resources\\stage3_elements.png", 5, 5);
	this->_listSprite[eID::ROCKFLY] = sp;
	this->loadSpriteInfo(eID::ROCKFLY, "Resources\\rockfly_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\Life.png");
	this->_listSprite[eID::LIFE_ICON] = sp;
	this->loadSpriteInfo(eID::LIFE_ICON, "Resources\\life_info.txt");


	sp = new Sprite(spriteHandle, L"Resources\\ScubaSoldier.png");
	this->_listSprite[eID::SCUBASOLDIER] = sp;
	this->loadSpriteInfo(eID::SCUBASOLDIER, "Resources\\scubasoldier_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\stage3_elements.png", 5, 5);
	this->_listSprite[eID::FIRE] = sp;
	this->loadSpriteInfo(eID::FIRE, "Resources\\fire_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\rockfall.png", 4, 4);
	this->_listSprite[eID::ROCKFALL] = sp;
	this->loadSpriteInfo(eID::ROCKFALL, "Resources\\rockfall_animation.txt");

	sp = new Sprite(spriteHandle, L"Resources\\Fonts\\fontEx.png", 30, 10);
	this->_listSprite[eID::FONTEX] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\Fonts\\fontFull.png", 54, 6);
	this->_listSprite[eID::FONTFULL] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\BeginState3.png");
	this->_listSprite[eID::BEGIN_STAGE3] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\GameOver.png");
	this->_listSprite[eID::GAME_OVER_SCENE] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\blank.png");
	this->_listSprite[eID::BLANK] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\intro.png");
	this->_listSprite[eID::MENU] = sp;

	sp = new Sprite(spriteHandle, L"Resources\\yellowfalcon.png");
	this->_listSprite[eID::YELLOWFALCON] = sp;

	// Đọc file xml để tạo đối tượng sprite
	sp = loadXMLDoc(spriteHandle, L"Resources//Map//stage1.xml");
	sp->setOrigin(VECTOR2ZERO);
	this->_listSprite[eID::MAPSTAGE1] = sp;
}

Sprite* SpriteManager::loadXMLDoc(LPD3DXSPRITE spritehandle, LPWSTR path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path, pugi::parse_default  | pugi::parse_pi);
	if (result == false)
	{
		OutputDebugString(L"Khong tim thay file xml");
		return nullptr;
	}
	pugi::xml_node root = doc.first_child();
	pugi::xml_node tileset_node = root.child("TileSet");
	// Tìm tên file.
	// Cắt từ chuỗi path ra để tìm thư mục.
	// Sau đó ghép với tên file ảnh được lấy từ file xml để load ảnh.
	string filename = tileset_node.attribute("FileName").as_string();   // get filename from xml node
	wstring L_filename = wstring(filename.begin(), filename.end());		// convert to wstring.

	wstring strpath = wstring(path);									// convert to wstring.
	int index = strpath.find_last_of(L'//');							// cut to find path
	strpath = strpath.substr(0, index);
	strpath +=  L"/" + L_filename;										// concat string.  Final string is strpath.
	// Tìm số dòng
	int rows = tileset_node.attribute("Rows").as_int();
	int columns = tileset_node.attribute("Columns").as_int();
	return new Sprite(spritehandle,(LPWSTR) strpath.c_str(), rows * columns, columns);
}
Sprite* SpriteManager::getSprite(eID id)
{
	Sprite *it = this->_listSprite.find(id)->second;
	return new Sprite(*it);			// get the copy version of Sprite
}

RECT SpriteManager::getSourceRect(eID id, string name)
{
	//return _sourceRectList[id].at(name);
	return _sourceRectList[id][name]; 
}

void SpriteManager::loadSpriteInfo(eID id, const char* fileInfoPath)
{
	FILE* file;
	file = fopen(fileInfoPath, "r");

	if (file)
	{
		while (!feof(file))
		{
			RECT rect;
			char name[100];
			fgets(name, 100, file);

			fscanf(file, "%s %d %d %d %d", &name, &rect.left, &rect.top, &rect.right, &rect.bottom);

			_sourceRectList[id][string(name)] = rect;
		}
	}

	fclose(file);
}

void SpriteManager::releaseSprite(eID id)
{
	Sprite *it = this->_listSprite.find(id)->second;
	delete it;							// delete the sprite only, dont relase image
	this->_listSprite.erase(id);		// erase funciotn only remove the pointer from MAP, dont delete it.
}
void SpriteManager::releaseTexture(eID id)
{
	Sprite *spr = this->_listSprite.find(id)->second;
	spr->release();						// release image
	delete spr;							
	this->_listSprite.erase(id);		// erase funciotn only remove the pointer from MAP, dont delete it.
}
SpriteManager* SpriteManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}

void SpriteManager::release()
{
	delete _instance;			// _instance is static attribute, only static function can delete it.
	_instance = nullptr;
}
