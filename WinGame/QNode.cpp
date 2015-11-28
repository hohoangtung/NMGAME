#include "QNode.h"


QNode::QNode(int id, int level, RECT bound, QNode* parent)
{
	this->_id = id;
	this->_level = level;
	this->_bound = bound;
	this->_parent = parent;

}

void QNode::insertObject(BaseObject* baseobject)
{
	this->_listOject.push_back(baseobject);
}

RECT QNode::getBound()
{
	return _bound;
}
INT64 QNode::getId()
{
	return _id;
}
int QNode::getLevel()
{
	return _level;
}
QNode* QNode::getParent()
{
	return _parent;
}
void QNode::setParent(QNode* parent)
{
	this->_parent = parent;
}
vector<BaseObject*> QNode::getAllObject()
{
	return	_listOject;
}

bool QNode::isLeaf()
{
	if (_childs[0] == nullptr)
		return true;
	return false;
}

vector<BaseObject*> QNode::getlistObject(RECT bound)
{
	vector<BaseObject*> rs;
	if (isContains(this->_bound, bound) || isRectangleIntersected(this->_bound, bound) || isContains(bound, this->_bound))
	{
		if (this->isLeaf() == true)
		{
			if (this->_listOject.empty() == false)
				rs.insert(rs.end(), this->_listOject.begin(), this->_listOject.end());
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				vector<BaseObject*> temp = _childs[i]->getlistObject(bound);
				if (temp.empty() == false)
				rs.insert(rs.end(), this->_listOject.begin(), this->_listOject.end());
			}
		}

	}
	return rs;
}

QNode* QNode::loadQuadTree(const string path, map<string, BaseObject*> listobject)
{
	QNode* node = nullptr;
	pugi::xml_document doc;

	// Mở file và đọc
	xml_parse_result result = doc.load_file(path.data(), pugi::parse_default | pugi::parse_pi);
	if (result == false)
	{
		return nullptr;
	}
	pugi::xml_node rootxml = doc.first_child();
	node = initNode(rootxml);
	loadChild(rootxml, node, listobject);
	return node;
}
QNode* QNode::initNode(xml_node& node )
{
	RECT bound;
	int id = node.attribute("Id").as_int();
	int level = node.attribute("Level").as_int();
	bound.top = node.attribute("Y").as_int();
	bound.left = node.attribute("X").as_int();
	bound.right = bound.left + node.attribute("Width").as_int();
	bound.bottom = bound.top + node.attribute("Height").as_int();
	QNode* qnode = new QNode(id, level, bound, NULL);
	return qnode;
}
void QNode::loadChild(pugi::xml_node& node,QNode* parent, map<string, BaseObject*> listobject)
{
	QNode* child[4];
	auto childnodes = node.children();
	string nodename;
	int childindex = 0;
	for each (xml_node childnode in childnodes)
	{

		nodename = childnode.name();
		if (nodename == "Objects")
		{
			string text = childnode.text().as_string();
			auto objectnames = splitString(text, ' ');
			for each (string name in objectnames)
			{
				auto obj = listobject[name];
				if (obj == NULL)
					continue;
				parent->insertObject(obj);
			}
		}
		else if (nodename == "QNode")
		{
			QNode* qnode = initNode(childnode);
			qnode->setParent(parent);
			loadChild(childnode, qnode, listobject);
			child[childindex] = qnode;
			childindex++;
		}
	}
	parent->setChilds(child);
}

QNode** QNode::getChilds()
{
	return _childs;
}

void QNode::setChilds(QNode* nodes[4])
{
	for (int i = 0; i < 4; i++)
	{
		this->_childs[i] = nodes[i];
	}
}
QNode::~QNode()
{
}
