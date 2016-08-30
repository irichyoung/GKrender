#include"gkdebug.h"
void GKDebug(const char*formation, ...)
{
	va_list list;
	va_start(list, formation);
	char head[3024] = "GKENGINE::";
	char string[3000];
	_vsnprintf_s(string, sizeof(string), formation, list);
	va_end(list);
	strcat_s(head, string);
	OutputDebugStringA(head);
}
std::string GKGetError(bool dx)
{
	if (!dx){
		auto result = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, result, NULL, (LPSTR)&lpMsgBuf, 1000, NULL);
		std::string tmp((LPSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return tmp;
	}
	_com_error e(dx);
	const TCHAR * tmp = const_cast<TCHAR*>(e.ErrorMessage());
	int num = WideCharToMultiByte(CP_ACP, NULL, tmp, -1, NULL, NULL, NULL, NULL);
	char tmpp[1000];
	WideCharToMultiByte(CP_ACP, NULL, tmp, -1, tmpp, num, NULL, NULL);
	return std::string(tmpp);
}