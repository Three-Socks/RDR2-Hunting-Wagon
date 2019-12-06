#include "script.h"
#include "log.h"

#include <Shlobj.h>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

void wagon_get_log_file_path(char* log_file, char* log_file_path)
{
	char path[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	if (hr == S_OK)
		sprintf_s(log_file_path, MAX_PATH, "%s\\Rockstar Games\\Red Dead Redemption 2\\%s.log", path, log_file);
	else
		sprintf_s(log_file_path, MAX_PATH, "%s.log", log_file);
}

char g_logFile[MAX_PATH];
bool Log::s_bConsole;
void Log::Init(bool createConsole)
{
	FILE* file;
	memset(g_logFile, 0, sizeof(g_logFile));

	wagon_get_log_file_path(CONFIG_NAME, g_logFile);

	if ((fopen_s(&file, g_logFile, "w")) == 0)
	{
		fprintf_s(file, "%s\n", CONFIG_NAME);
		fclose(file);
	}

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