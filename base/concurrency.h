#ifndef __GK_BASE_CONCURRENCY__
#define __GK_BASE_CONCURRENCY__
#include<thread>
#include<Windows.h>
#include<mutex>
#include<iostream>
namespace gk{
	namespace base{
		class gkmutex{
#if _MSC_VER > 1800
			mutex m_mutex;
		public:
			void lock(){
				m_mutex.lock();
			}
			void unlock(){
				m_mutex.unlock();
			}
		};
		mutex gklock::m_mutex;
#else
			CRITICAL_SECTION cs_m;
		public:
			gkmutex(){
				InitializeCriticalSectionAndSpinCount(&cs_m, 1000);
			}
			~gkmutex(){
				DeleteCriticalSection(&cs_m);
			}
			void lock(){
				EnterCriticalSection(&cs_m);
			}
			void unlock(){
				LeaveCriticalSection(&cs_m);
			}
	};
#endif
		class gklock{
			gkmutex &mutex;
		public:
			gklock(gkmutex&gkm) :mutex(gkm){
				mutex.lock();
			}
			~gklock(){
				mutex.unlock();
			}
		};
	}
}
#endif