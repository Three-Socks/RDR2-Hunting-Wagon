#include "log.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

char g_logFile[MAX_PATH];
bool Log::s_bConsole;
void Log::Init(bool createConsole)
{
	FILE* file;
	memset(g_logFile, 0, sizeof(g_logFile));

	//if (GetCurrentDirectoryA(sizeof(g_logFile), g_logFile))
	//{
		strcat_s(g_logFile, "C:\\rdr2\\wagonLog.txt");
		if ((fopen_s(&file, g_logFile, "w")) == 0)
		{
			fprintf_s(file, "Wagon\n");
			fclose(file);
		}
		else
		{
			MessageBoxA(0, "Failed to open riotLog.txt", "FATAL ERROR", MB_ICONERROR);
		}
	//}
	/*else
	{
		MessageBoxA(NULL, "GetCurrentDirectory failed", "ERROR", MB_OK);
		ExitProcess(0);
	}*/
	if (createConsole)
	{
		HWND handle = GetConsoleWindow();
		if (!handle)
		{
			ShowWindow(handle, SW_RESTORE);

			ShowWindow(handle, SW_MAXIMIZE);

			ShowWindow(handle, SW_SHOW);
		}
		else
		{
			AllocConsole();
			AttachConsole(GetCurrentProcessId());
		}
		s_bConsole = true;
	}
}

void Log::Write(Log::Type type, const char* format, ...)
{
	FILE* file;
	va_list message;
	char timestamp[25], logType[15], logBuffer[4096], logMessage[4096];
	struct tm *sTm;

	time_t now = time(0);
	sTm = localtime(&now);

	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", sTm);
	memset(logType, 0, sizeof(logType));
	switch (type)
	{
	case Log::Type::Normal:
		strcpy_s(logType, "Normal");
		break;
	case Log::Type::Debug:
		strcpy_s(logType, "Debug");
#ifndef _DEBUG
		return;
#endif // !DEBUG
		break;
	case Log::Type::Error:
		strcpy_s(logType, "Error");
		break;
	case Log::Type::FatalError:
		strcpy_s(logType, "Fatal Error");
		break;

	}

	va_start(message, format);
	_vsnprintf_s(logBuffer, sizeof(logBuffer), format, message);
	va_end(message);
	sprintf_s(logMessage, "[%s][%s]: %s", timestamp, logType, logBuffer);
	if ((fopen_s(&file, g_logFile, "a")) == 0)
	{
		fprintf_s(file, "%s \n", logMessage);
		fclose(file);
		if (Log::s_bConsole)
			printf_s("%s \n", logMessage);
		if (type == Log::Type::Error || type == Log::Type::FatalError)
		{
			MessageBoxA(NULL, logMessage, logType, MB_ICONERROR);
			if (type == Log::Type::FatalError)
			{
				ExitProcess(0);
			}
		}
	}
}