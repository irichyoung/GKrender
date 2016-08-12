#ifndef __GK_BASE_CONCURRENCY__
#define __GK_BASE_CONCURRENCY__
#include<thread>
#include<Windows.h>
#include<mutex>
#include<iostream>
#include<sstream>
#include"gkdebug.h"
namespace gkconcurrency{
	class init_cs{
	public:
		CRITICAL_SECTION g_cs;
		init_cs(){
			GKDebug("File:%s\nFunction:%s\nLine:%d\n", __FILE__, __FUNCTION__, __LINE__);
			InitializeCriticalSectionAndSpinCount(&g_cs, 1000);
		}
		~init_cs(){
			DeleteCriticalSection(&g_cs);
		}
	};
	extern init_cs _ics;
	class gklock{
#if _MSC_VER > 1800
		static std::mutex m_mutex;
	public:
		gklock(){
			m_mutex.lock();
		}
		~gklock(){
			m_mutex.unlock();
		}
	};
	std::mutex gklock::m_mutex;
#else
	public:
		gklock(){
			EnterCriticalSection(&_ics.g_cs);
		}
		~gklock(){
			LeaveCriticalSection(&_ics.g_cs);
		}
};
#endif
}

#endif