

#ifndef __OBJECT_READER__
#define __OBJECT_READER__

#include "..\FrameWork\define.h"
#include "..\objects\BaseObject.h"
#include "..\pugixml-1.7\src\pugixml.hpp"
#include "..\objects\Rifleman.h"

using namespace pugi;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	static vector<BaseObject*>* getListObjectFromFile(const string path);
private: 
	static BaseObject* getObjectById(xml_node node, eID id);

	static BaseObject* getLand(xml_node node);
	static BaseObject* getRifleMan(xml_node node);

	static map<string, int> getObjectProperties(xml_node node);

};

#endif // !__OBJECT_READER__
