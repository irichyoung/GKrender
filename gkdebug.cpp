#include"gkdebug.h"
void GKDebug(const char * formation, ...)
{
	va_list list;
	va_start(list, formation);
	char head[1024] = "GKENGINE::";
	char string[1000];
	_vsnprintf_s(string, sizeof(string), formation, list);
	va_end(list);
	strcat_s(head, string);
	OutputDebugStringA(head);
}
void GKGetError()
{
	auto result = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, result, NULL, (LPSTR)&lpMsgBuf, 1000, NULL);
	GKDebug((LPSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}