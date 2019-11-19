#pragma once
#include<windows.h>
#include<string>
#include<vector>

void Tokenize(std::vector<std::wstring>& tokens, const std::wstring& str, const std::wstring& delimiters = _T(" "));
std::wstring TrimString(const std::wstring& string, const WCHAR*trimCharacterList = _T(" \t\v\r\n"));

