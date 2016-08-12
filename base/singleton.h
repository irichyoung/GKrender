#ifndef __GK_BASE_SINGLETON__
#define __GK_BASE_SINGLETON__
#include"concurrency.h"
#define singleton(_class_name_)						\
	public:											\
		 static	 _class_name_*getinstance(){		\
			static _class_name_* instance;			\
			if(!instance){							\
					{								\
					gkconcurrency::gklock();		\
					if (!instance)					\
					instance = new _class_name_();	\
					};								\
			}										\
			return instance;						\
			}										\

#endif