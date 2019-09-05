#pragma once
#include "stdafx.h"

class Debug
{
public:
	//constructor
	Debug();
	~Debug();

	//static void Log(TCHAR* str);

	/*template <typename... Args>
	static void Log(char* str, Args&... args);*/
	template <typename... Args>
	static void Log(TCHAR* str, const Args&... args);

	template <typename... Args>
	static void Log(TCHAR* str, Args&... args);
};

//void Debug::Log(TCHAR* str) {
//	TCHAR debugStr[512];
//	_stprintf_s(debugStr, 512, str);
//	OutputDebugString(debugStr);
//}

template <typename... Args>
void Debug::Log(TCHAR* str, const Args&... args) {
	TCHAR debugStr[512];
	_stprintf_s(debugStr, 512, str, args...);
	OutputDebugString(debugStr);
}

template <typename... Args>
void Debug::Log(TCHAR* str, Args&... args) {
	TCHAR debugStr[512];
	_stprintf_s(debugStr, 512, str, args...);
	OutputDebugString(debugStr);
}