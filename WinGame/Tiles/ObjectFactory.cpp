
#include "ObjectFactory.h"
#include "..\objects\Land.h"

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
		case BILL:
			break;
		case REDCANNON:
			return getRedCannon(node);
			break;
		case SOLDIER:
			return getSoldier(node);
			break;
		case FALCON:
			break;
		case AIRCRAFT:
			return getAirCraft(node);
			break;
		case EXPLOSION:
			break;
		case RIFLEMAN:
			return getRifleMan(node);
			break;
		case BRIDGE:
			break;
		case QUADEXPLODE:
			break;
		case MAPSTAGE1:
			break;
		case BULLET:
			break;
		case LAND:
			return getLand(node);
			break;
		case WALL_TURRET:
			return getWallTurret(node);
			break;
		default:
			return NULL;
			break;
		}
}

BaseObject* ObjectFactory::getLand(xml_node node)
{
	Land* land = NULL;
	int x, y, width, height;
	x = node.attribute("X").as_int();
	y = node.attribute("Y").as_int();
	width = node.attribute("Width").as_int();
	height = node.attribute("Height").as_int();
	
	map<string, string> parametter;
	xml_node params = node.child("Params");
	string key = "";
	string value = "";
	for (auto item : params)
	{
		key = item.attribute("Key").as_string();
		value = item.attribute("Value").as_string();
		parametter.insert(pair<string,string>(key, value));
	}

	eDirection dir;
	eLandType type;
	try
	{
		dir = (eDirection) std::stoi(parametter.find("physicBodyDirection")->second);
	}
	catch(exception e)
	{
		dir = eDirection::TOP;
	}

	try
	{
		type = (eLandType) std::stoi(parametter.find("type")->second);
	}
	catch(exception e)
	{
		type = eLandType::GRASS;
	}
	land = new Land(x, y, width, height, dir, type);
	land->init();
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

	try
	{
		status = (eStatus)stoi(properties["status"]);
	}
	catch (exception ex)
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
	
	try
	{
		status = (eStatus)stoi(properties["status"]);
	}
	catch (exception ex)
	{
		status = eStatus::NORMAL;
	}

	try
	{
		dir = stoi(properties["direction"]);
	}
	catch (exception ex)
	{
		dir = -1;
	}

	auto soldier = new Soldier(status, GVector2(x, y), dir);
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

	try
	{
		status = (eStatus)stoi(properties["status"]);
	}
	catch (exception ex)
	{
		status = eStatus::NORMAL;
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

	try
	{
		status = (eStatus)(stoi(properties["status"]));
	}
	catch (exception ex)
	{
		status = eStatus::NORMAL;
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

	try
	{
		type = (eAirCraftType)(stoi(properties["type"]));
	}
	catch (exception ex)
	{
		type = eAirCraftType::S;
	}

	// ampl
	try
	{
		auto velocStr = properties["Amplitude"];
		auto value = splitString(velocStr, ',');

		ampl.x = stoi(value[0]);
		ampl.y = stoi(value[1]);
	}
	catch (exception ex)
	{
		ampl = AIRCRAFT_AMPLITUDE;
	}

	// hVeloc
	try
	{
		auto velocStr = properties["HVelocity"];
		auto value = splitString(velocStr, ',');

		hVeloc.x = stoi(value[0]);
		hVeloc.y = stoi(value[1]);
	}
	catch (exception ex)
	{
		hVeloc = HORIZONTAL_VELOC;
	}

	// freq
	try
	{
		freq = stof(properties["frequency"]);
	}
	catch (exception ex)
	{
		freq = AIRCRAFT_FREQUENCY;
	}

	auto airCraft = new AirCraft(pos, hVeloc, ampl, freq, type);
	airCraft->init();

	return airCraft;
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

