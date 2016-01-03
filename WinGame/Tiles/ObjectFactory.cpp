
#include "ObjectFactory.h"
#include "../objects/Land.h"
#include "../objects/Enemies/RockCreator.h"
#include "../objects/Enemies/ShadowBeast.h"

ObjectFactory::ObjectFactory()
{
}

ObjectFactory::~ObjectFactory()
{
}

vector<BaseObject*>* ObjectFactory::getListObjectFromFile(const string path)
{
	pugi::xml_document doc;
	vector<BaseObject*>* listobject = new vector<BaseObject*>();

	// Mở file và đọc
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default  | pugi::parse_pi);
		if (result == false)
	{
		return listobject; 
	}

	xml_node tilemap = doc.child("Tilesmap");
	if (tilemap == NULL)
		return listobject;

	xml_node objects = tilemap.child("Objects");
	auto list = objects.children();

	// Lấy id từ file xml. so sánh với eID, tuỳ theo eID nào mà gọi đến đúng hàm load cho riêng object đó.
	for(auto item : list)
	{
		int id = item.attribute("Id").as_int();
		eID enumID;
		try{
			enumID = (eID)id;
		}
		catch (exception e){
			continue;
		}
		BaseObject* obj = getObjectById(item, enumID);
		obj->setZIndex(0.5f);
		if (obj != NULL)
			listobject->push_back(obj);
	}

	return listobject;
}

map<string, BaseObject*>* ObjectFactory::getMapObjectFromFile(const string path)
{
	pugi::xml_document doc;
	map<string, BaseObject*>* listobject = new map<string, BaseObject*>();

	// Mở file và đọc
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return listobject;
	}

	xml_node tilemap = doc.child("Tilesmap");
	if (tilemap == NULL)
		return listobject;

	xml_node objects = tilemap.child("Objects");
	auto list = objects.children();

	// Lấy id từ file xml. so sánh với eID, tuỳ theo eID nào mà gọi đến đúng hàm load cho riêng object đó.
	for (auto item : list)
	{
		int id = item.attribute("Id").as_int();
		string name = item.attribute("Name").as_string();
		eID enumID;
		try{
			enumID = (eID)id;
		}
		catch (exception e){
			continue;
		}
		BaseObject* obj = getObjectById(item, enumID);
		if (obj != NULL)
			(*listobject)[name] = obj;
	}
	return listobject;
}


BaseObject* ObjectFactory::getObjectById(xml_node node, eID id)
{
		switch (id)
		{
		case REDCANNON:
			return getRedCannon(node);
		case SOLDIER:
			return getSoldier(node);
		case FALCON:
			return getFalcon(node);
		case AIRCRAFT:
			return getAirCraft(node);
		case RIFLEMAN:
			return getRifleMan(node);
		case BRIDGE:
			return getBridge(node);
		case LAND:
			return getLand(node);
		case WALL_TURRET:
			return getWallTurret(node);
		case CREATOR:
			return getCreator(node);
		case BOSS_STAGE1:
			return getGreatWall(node);
		case ROCKFLY:
			return getRockFly(node);
		case ROCKFALL:
			return getRockFall(node);
		case SCUBASOLDIER:
			return getScubaSoldier(node);
		case FIRE:
			return getFire(node);
		case SHADOW_BEAST:
			return getShadowBeast(node);
		default:
			return nullptr;
			break;
		}
		return nullptr;
}

BaseObject* ObjectFactory::getLand(xml_node node)
{
	//Land* land = NULL;
	//int x, y, width, height;
	//x = node.attribute("X").as_int();
	//y = node.attribute("Y").as_int();
	//width = node.attribute("Width").as_int();
	//height = node.attribute("Height").as_int();
	//
	//map<string, string> parametter;
	//xml_node params = node.child("Params");
	//string key = "";
	//string value = "";
	//for (auto item : params)
	//{
	//	key = item.attribute("Key").as_string();
	//	value = item.attribute("Value").as_string();
	//	parametter.insert(pair<string,string>(key, value));
	//}

	//eDirection dir;
	//eLandType type;
	//try
	//{
	//	dir = (eDirection) std::stoi(parametter.find("physicBodyDirection")->second);
	//}
	//catch(exception e)
	//{
	//	dir = eDirection::TOP;
	//}

	//try
	//{
	//	type = (eLandType) std::stoi(parametter.find("type")->second);
	//}
	//catch(exception e)
	//{
	//	type = eLandType::GRASS;
	//}
	//land = new Land(x, y, width, height, dir, type);
	//land->init();
	//return land;

	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, width, height;
	eDirection dir;
	eLandType type;
	bool canJumpDown;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);

	if (properties.find("type") != properties.end())
	{
		type = (eLandType)(stoi(properties.find("type")->second));
	}
	else
	{
		type = eLandType::GRASS;
	}

	if (properties.find("physicBodyDirection") != properties.end())
	{
		dir = (eDirection)(stoi(properties.find("physicBodyDirection")->second));
	}
	else
	{
		dir = eDirection::TOP;
	}

	if (properties.find("canJumpDown") != properties.end())
	{
		canJumpDown = (bool)(stoi(properties.find("canJumpDown")->second));
	}
	else
	{
		canJumpDown = true;
	}

	auto land = new Land(x, y, width, height, dir, type);
	land->init();
	land->enableJump(canJumpDown);

	return land;
}

BaseObject * ObjectFactory::getRifleMan(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	if (properties.find("status") != properties.end())
	{
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else
	{
		status = eStatus::NORMAL;
	}

	auto rifleMan = new Rifleman(status, GVector2(x, y));
	rifleMan->init();

	return rifleMan;
}

BaseObject * ObjectFactory::getSoldier(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, dir;
	eStatus status;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	
	if (properties.find("status") != properties.end())
	{
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else
	{
		status = eStatus::RUNNING;
		//status = eStatus::NORMAL;
	}

	if (properties.find("direction") != properties.end())
	{
		dir = stoi(properties.find("direction")->second);
	}
	else
	{
		dir = -1;
	}

	bool canShoot = false;

	if (properties.find("canShoot") != properties.end())
	{
		auto v = properties.find("canShoot")->second;
		if (v == "true")
		{
			canShoot = true;
		}
	}

	auto soldier = new Soldier(status, GVector2(x, y), dir, canShoot);
	soldier->init();

	return soldier;
}

BaseObject * ObjectFactory::getRedCannon(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	eStatus status;

	x = stoi(properties["X"]) + 32;
	y = stoi(properties["Y"]) - 32;

	if (properties.find("status") != properties.end())
	{
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else
	{
		status = eStatus::WAITING;
	}

	auto cannon = new RedCannon(status, GVector2(x, y));
	cannon->init();

	return cannon;
}

BaseObject * ObjectFactory::getWallTurret(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	eStatus status;

	x = stoi(properties["X"]) + 32;
	y = stoi(properties["Y"]) - 32;

	if (properties.find("status") != properties.end())
	{
		status = (eStatus)(stoi(properties.find("status")->second));
	}
	else
	{
		status = eStatus::WAITING;
	}

	auto turret = new WallTurret(status, GVector2(x, y));
	turret->init();

	return turret;
}

BaseObject * ObjectFactory::getAirCraft(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	GVector2 pos, hVeloc, ampl;
	float freq;
	eAirCraftType type;

	pos.x = stoi(properties["X"]);
	pos.y = stoi(properties["Y"]);

	// type
	if (properties.find("type") != properties.end())
	{
		type = (eAirCraftType)(stoi(properties.find("type")->second));
	}
	else
	{
		type = eAirCraftType::S;
	}

	// ampl
	if (properties.find("Amplitude") != properties.end())
	{
		auto velocStr = properties.find("Amplitude")->second;
		auto value = splitString(velocStr, ',');

		ampl.x = stoi(value[0]);
		ampl.y = stoi(value[1]);
	}
	else
	{
		ampl = AIRCRAFT_AMPLITUDE;
	}

	// hVeloc
	if (properties.find("HVelocity") != properties.end())
	{
		auto velocStr = properties.find("HVelocity")->second;
		auto value = splitString(velocStr, ',');

		hVeloc.x = stoi(value[0]);
		hVeloc.y = stoi(value[1]);
	}
	else
	{
		hVeloc = AIRCRAFT_HORIZONTAL_VELOC;
	}

	// freq
	if (properties.find("Frequency") != properties.end())
	{
		freq = stof(properties.find("Frequency")->second);
	}
	else
	{
		freq = AIRCRAFT_FREQUENCY;
	}

	auto airCraft = new AirCraft(pos, hVeloc, ampl, freq, type);

	airCraft->init();

	return airCraft;
}

BaseObject * ObjectFactory::getCreator(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	GVector2 pos;
	int dir, num, width, height;
	eID type;
	float time;
	bool oneperone;

	pos.x = stoi(properties["X"]) + 32;
	pos.y = stoi(properties["Y"]) - 32;
	width = stoi(properties["Width"]);
	height = stoi(properties["Height"]);
	
	// type
	if (properties.find("type") != properties.end())
	{
		type = (eID)(stoi(properties.find("type")->second));
	}
	else
	{
		type = eID::SOLDIER;
	}

	// dir
	if (properties.find("direction") != properties.end())
	{
		dir = stoi(properties.find("direction")->second);
	}
	else
	{
		dir = -1;
	}

	// time
	if (properties.find("time") != properties.end())
	{
		time = stof(properties.find("time")->second);
	}
	else
	{
		time = 1000.0f;
	}

	// num
	if (properties.find("number") != properties.end())
	{
		num = stoi(properties.find("number")->second);
	}
	else
	{
		num = -1;
	}

	// one per one
	if (properties.find("oneperone") != properties.end())
	{
		auto v = properties.find("oneperone")->second;
		if (v == "true")
		{
			oneperone = true;
		}
		else
		{
			oneperone = false;
		}
	}
	else
	{
		oneperone = false;
	}

	// max num
	int maxNum = 2;
	if (properties.find("maxNumber") != properties.end())
	{
		maxNum = stoi(properties.find("maxNumber")->second);
	}

	eMapType mapType = eMapType::HORIZONTAL;
	if (properties.find("mapType") != properties.end())
	{
		mapType = (eMapType)stoi(properties.find("mapType")->second);
	}

	auto creator = new ObjectCreator(pos, width, height, type, dir, time, num);
	creator->setOnePerOne(oneperone);
	creator->setMaxNumber(maxNum);
	creator->setMapType(mapType);
	creator->init();

	return creator;
}

BaseObject* ObjectFactory::getBridge(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y;
	
	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	
	// X Cộng 16 Y Trừ 16 vì gameobject set origin là center
	auto bridge = new Bridge(GVector2(x + 16, y - 16));
	bridge->init();

	return bridge;
}

BaseObject* ObjectFactory::getFalcon(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, type;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	
	if (properties.find("type") != properties.end())
	{
		type = stoi(properties.find("type")->second);
	}
	else
	{
		type = eAirCraftType::R;
	}
	auto falcon = new Falcon(GVector2(x + 32, y - 32), (eAirCraftType)type);

	falcon->init();
	return falcon;
}

BaseObject* ObjectFactory::getGreatWall(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, height;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]) - stoi(properties["Height"]);
	height = stoi(properties["Height"]);

	auto greatwall = new Boss(GVector2(x, y), height);
	greatwall->init();
	return greatwall;
}

BaseObject* ObjectFactory::getRockFly(xml_node node)
{
	xml_node activebound = node.child("ActiveBound");

	// general
	int x = activebound.attribute("X").as_int();
	int y = activebound.attribute("Y").as_int();
	int width = activebound.attribute("Width").as_int();

	x += 32;
	width -= 64;
	y -= 32;

	auto rockfly = new RockFly(GVector2(x, y), GVector2(x + width, y));
	rockfly->init();
	return rockfly;
}
BaseObject* ObjectFactory::getRockFall(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, type;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	auto rockfall = new RockCreator(GVector2(x + 32, y - 32));
	rockfall->init();
	return rockfall;
}

BaseObject* ObjectFactory::getScubaSoldier(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, type;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	x += 32 ;
	y -= 8 ;
	auto scubasoldier = new ScubaSoldier(GVector2(x , y));
	scubasoldier->init();
	return scubasoldier;
}

BaseObject* ObjectFactory::getShadowBeast(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, type;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);

	auto shadowbeast = new ShadowBeast(GVector2(x, y));
	shadowbeast->init();
	return shadowbeast;
}

BaseObject* ObjectFactory::getFire(xml_node node)
{
	auto properties = getObjectProperties(node);
	if (properties.size() == 0)
		return nullptr;

	int x, y, type;

	x = stoi(properties["X"]);
	y = stoi(properties["Y"]);
	
	y -= 8 * SCALE_FACTOR;
	auto fire = new Fire(GVector2(x, y));
	fire->init();
	return fire;
}

map<string, string> ObjectFactory::getObjectProperties(xml_node node)
{
	map<string, string> properties;

	// general
	properties["X"] = node.attribute("X").as_string();
	properties["Y"] = node.attribute("Y").as_string();
	properties["Width"] = node.attribute("Width").as_string();
	properties["Height"] = node.attribute("Height").as_string();

	// parameters
	xml_node params = node.child("Params");
	for (auto item : params)
	{
		auto key = item.attribute("Key").as_string();
		auto value = item.attribute("Value").as_string();
		properties[key] = value;
	}

	return properties;
}

