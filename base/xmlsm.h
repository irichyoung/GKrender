#ifndef __XML_STATE_MACHINE__
#define __XML_STATE_MACHINE__
#include<algorithm>
#include<vector>
#include<string>
#include<map>
#include<set>
#include"base/tinyxml2.h"
#include<functional>
using namespace std;
namespace gk{
	namespace base{
		struct condition{
			string state;
			string input;
			condition(){};
			condition(const string &s, const string &i) :state(s), input(i){};
			friend bool operator < (const condition &l, const condition&r){
				if (l.state != r.state){
					return l.state < r.state;
				}
				else {
					return l.input < r.input;
				}
			}
		};
		class xmlsm{
		private:
			map<condition, function<void()>> _rules;
			set<string>_keys;
			string _filename;
			tinyxml2::XMLDocument _doc;
			tinyxml2::XMLElement *_input;
			void traverse(const tinyxml2::XMLElement *e);
		public:
			void connect(const condition &con, const function<void()>&func){
				_rules[con] = func;
				_keys.insert(con.input);
				_keys.insert(con.input);
			}
			void connect(const string&name){
				_keys.insert(name);
			}
			void disconnect(const condition &con){
				_rules.erase(con);
				_keys.erase(con.input);
				_keys.erase(con.state);
			}
			void disconnect(const string &con){
				_keys.erase(con);
			}
			void loadfile(const string &file){
				_doc.LoadFile(file.c_str());
			}
			void start(){
				_input = _doc.FirstChildElement();
				traverse(_input);
			}
			tinyxml2::XMLElement* getElement(){
				return _input;
			}
		};
	}
}
#endif