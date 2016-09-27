#ifndef __GK_BASE_MACROS__
#define __GK_BASE_MACROS__
#include<string>
#include<Windows.h>

#define GET(x,y) private:x y;public:inline x get##y(){return y;};
#define GETP(x,y) private:x y;public:inline x* get##y(){return &y;};
#define SET(x,y) public:inline void set##y(x a){y = a;};
#define GET_SET(x,y) GET(x,y) SET(x,y)
#define GETP_SET(x,y) GETP(x,y) SET(x,y)

#define GET_STATIC(x,y) private:static x y;public:inline x get##y(){return y;};
#define GETP_STATIC(x,y) private:static x y;public:inline x* get##y(){return &y;};
#define GET_SET_STATIC(x,y) GET_STATIC(x,y) SET(x,y)
#define GETP_SET_STATIC(x,y) GETP_STATIC(x,y) SET(x,y)

#define PRINT(y) public:inline void print##y(){cout<<"##y:"<<y<<endl;};
#define GET_SET_PRINT(x,y) private:x y; GET(x,y) SET(x,y) PRINT(y)

#define FUN_ERR(exp)\
	if (exp != S_OK){\
		GKDebug("line:%d,fuction:%s,error:%s\n", __LINE__, __FUNCTION__, GKGetError(true).c_str());\
		return false;\
	}
std::wstring UTF8TOWIDECHAR(std::string &);
std::wstring ANSITOWIDECHAR(std::string &);
std::string WCHARTOUTF8(std::wstring &);
std::string WCHARTOANSI(std::wstring &);

#endif