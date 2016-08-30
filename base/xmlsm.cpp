#include"xmlsm.h"
using namespace gk::base;
void xmlsm::traverse(const tinyxml2::XMLElement *e){
	if (e == NULL)return;
	for (tinyxml2::XMLElement *child = const_cast<tinyxml2::XMLElement*>(e->FirstChildElement()); child != NULL; child = child->NextSiblingElement()){//�����ýڵ���ӽڵ�
		_input = child;
		if (_keys.find(child->Value()) != _keys.end()){//����иýڵ��¼
			condition tmp(e->Value(), child->Value());
			if (_rules.find(tmp) != _rules.end()){//����й���
				_rules[tmp]();
			}
			traverse(child);
		}
	}
}