#include"macros.h"
std::wstring UTF8TOWIDECHAR(std::string &data){
	auto size = MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, NULL, 0);
	auto temp = new wchar_t[size + 1];
	memset(temp, 0, size + 1);
	MultiByteToWideChar(CP_UTF8, 0, data.c_str(), -1, LPWSTR(temp), size);
	std::wstring ws(temp);
	delete temp;
	return ws;
}
std::wstring ANSITOWIDECHAR(std::string &data){
	auto size = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, NULL, 0);
	auto temp = new wchar_t[size + 1];
	memset(temp, 0, size + 1);
	MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, LPWSTR(temp), size);
	std::wstring ws(temp);
	delete temp;
	return ws;
}

std::string WCHARTOUTF8(std::wstring &data){
	auto size = WideCharToMultiByte(CP_UTF8, 0, data.c_str(), -1, 0, 0, 0, 0);
	auto temp = new char[size + 1];
	memset(temp, 0, size + 1);
	WideCharToMultiByte(CP_UTF8, 0, data.c_str(), -1, temp, size, 0, 0);
	std::string s(temp);
	delete temp;
	return s;
}

std::string WCHARTOANSI(std::wstring &data){
	auto size = WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, 0, 0, 0, 0);
	auto temp = new char[size + 1];
	memset(temp, 0, size + 1);
	WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, temp, size, 0, 0);
	std::string s(temp);
	delete temp;
	return s;
}