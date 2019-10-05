#pragma once
#include<windows.h>


void DebugLog(const WCHAR *fmt, ...);
void DebugLog(const char *fmt, ...);
void DebugLogLastError(DWORD errorcode);


