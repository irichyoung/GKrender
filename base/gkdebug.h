#ifndef __GK_BASE_DEBUG__
#define __GK_BASE_DEBUG__
#include<stdio.h>
#include<stdarg.h>
#include<Windows.h>
#include<string>
#include<comdef.h>
void GKDebug(const char*, ...);
//true for dx,false for win32
std::string GKGetError(bool);
#endif