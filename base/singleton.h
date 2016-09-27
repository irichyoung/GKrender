#include"concurrency.h"
#include"gkdebug.h"
#ifndef __GK_BASE_SINGLETON__
#define __GK_BASE_SINGLETON__
extern gk::base::gkmutex singlemutex;
#define singleton(_class_name_)												\
	private:																\
		_class_name_(){};													\
		~_class_name_(){};													\
	public:																	\
		 static _class_name_*	getinstance(){								\
			static _class_name_* instance;									\
			if(!instance){													\
				gk::base::gklock singlelock(singlemutex);					\
				if (!instance){												\
					instance = new _class_name_();							\
					GKDebug("construct");									\
				}															\
			}																\
			return instance;												\
		}
#endif