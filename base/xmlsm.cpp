#include"xmlsm.h"
using namespace gk::base;
void xmlsm::traverse(const tinyxml2::XMLElement *e){
	if (e == NULL)return;
	for (tinyxml2::XMLElement *child = const_cast<tinyxml2::XMLElement*>(e->FirstChildElement()); child != NULL; child = child->NextSiblingElement()){//遍历该节点的子节点
		_input = child;
		if (_keys.find(child->Value()) != _keys.end()){//如果有该节点记录
			condition tmp(e->Value(), child->Value());
			if (_rules.find(tmp) != _rules.end()){//如果有规则
				_rules[tmp]();
			}
			traverse(child);
		}
	}
}