#include "stdafx.h"


void Tokenize(std::vector<std::wstring>& tokens, const std::wstring& str, const std::wstring& delimiters)
{
	tokens.clear();
	// Skip delimiters at beginning.
	std::wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::wstring::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::wstring::npos != pos || std::wstring::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void DebugLog(const WCHAR *fmt, ...)
{
	WCHAR     tmp[1024];
	va_list  ap;

	va_start(ap, fmt);
	wvsprintf(tmp, fmt, ap);

	// VisualStudioのデバッグウィンドウには必ず出力する
	OutputDebugString(tmp);	

	va_end(ap);
}