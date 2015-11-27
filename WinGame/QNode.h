#ifndef __QNODE_H__
#define __QNODE_H__

#include "FrameWork\define.h"
#include "objects\BaseObject.h"
#include "pugixml-1.7\src\pugixml.hpp"
using namespace pugi;
class QNode
{
private:
	RECT _bound;
	INT64 _id;
	int _level;
	QNode* _parent;
	QNode* _childs[4];
	vector<BaseObject*>  _listOject;
public:
	QNode(int id, int level, RECT bound, QNode* parent);
	void insertObject(BaseObject* baseobject);
	RECT getBound();
	INT64 getId();
	int getLevel();
	QNode* getParent();
	void setParent(QNode* parent);
	QNode** getChilds();
	void setChilds(QNode* nodes[4]);
	bool isLeaf();


	vector<BaseObject*> getAllObject();
	vector<BaseObject*> getlistObject(RECT bound);

	static void loadChild(xml_node& node, QNode* parent, map<string, BaseObject*> listobject);
	static QNode* loadQuadTree(const string path, map<string, BaseObject*> listobject);
	static QNode* initNode(xml_node& node);
	~QNode();
};



#endif // !__QNODE_H__

