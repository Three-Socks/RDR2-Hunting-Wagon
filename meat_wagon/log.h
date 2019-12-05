#pragma once
#ifndef LOG_H_
#define LOG_H_
#include <Windows.h>
#include <time.h>
#include <cstdio>

class Log
{
public:
	enum Type {
		Normal,
		Debug,
		Error,
		FatalError
	};
	static void Log::Init(bool CreateConsole = false);
	static void Log::Write(Log::Type type, const char* fmt, ...);
private:
	static bool s_bConsole;
};
#endif
