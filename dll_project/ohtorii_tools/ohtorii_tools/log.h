#pragma once
#include<windows.h>


void DebugLog(const WCHAR *fmt, ...);
void DebugLogLastError(DWORD errorcode);


