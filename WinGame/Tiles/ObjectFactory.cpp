
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

BaseObject* ObjectFactory::getObjectById(xml_node node, eID id)
{
		switch (id)
		{
		case BILL:
			break;
		case REDCANNON:
			break;
		case SOLDIER:
			break;
		case FALCON:
			break;
		case AIRCRAFT:
			break;
		case EXPLOSION:
			break;
		case RIFLEMAN:
			return getRifleMan(node);
			break;
		case BOX:
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
	int x, y;
	eStatus status;

	if (properties.size() == 0)
		return nullptr;

	try
	{
		status = (eStatus)properties["status"];
		x = properties["X"];
		y = properties["Y"];
	}
	catch (exception ex)
	{
		status = eStatus::NORMAL;
	}

	auto rifleMan = new Rifleman(status, GVector2(x, y));
	rifleMan->init();

	return rifleMan;
}

map<string, int> ObjectFactory::getObjectProperties(xml_node node)
{
	map<string, int> properties;

	// general
	properties["X"] = node.attribute("X").as_int();
	properties["Y"] = node.attribute("Y").as_int();
	properties["Width"] = node.attribute("Width").as_int();
	properties["Height"] = node.attribute("Height").as_int();

	// parameters
	xml_node params = node.child("Params");
	for (auto item : params)
	{
		auto key = item.attribute("Key").as_string();
		auto value = item.attribute("Value").as_int();
		properties[key] = value;
	}

	return properties;
}

