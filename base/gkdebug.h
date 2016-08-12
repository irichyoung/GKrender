#ifndef __GK_BASE_DEBUG__
#define __GK_BASE_DEBUG__
#include<stdio.h>
#include<stdarg.h>
#include<Windows.h>
void GKDebug(const char * formation, ...);
void GKGetError();
#endif