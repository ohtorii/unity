#pragma once
#include<windows.h>


void DebugLogEnable(bool enable);
void DebugLog(const WCHAR *fmt, ...);
void DebugLog(const char *fmt, ...);


void DebugLogLastError(DWORD errorcode);


