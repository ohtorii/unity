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

void DebugLogLastError(DWORD errorcode) {
	LPVOID lpMsgBuf=0;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER  //      テキストのメモリ割り当てを要求する
		| FORMAT_MESSAGE_FROM_SYSTEM    //      エラーメッセージはWindowsが用意しているものを使用
		| FORMAT_MESSAGE_IGNORE_INSERTS,//      次の引数を無視してエラーコードに対するエラーメッセージを作成する
		NULL, errorcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//   言語を指定
		(LPTSTR)&lpMsgBuf,                          //      メッセージテキストが保存されるバッファへのポインタ
		0,
		NULL);
	DebugLog(_T("%s"), lpMsgBuf);
	LocalFree(lpMsgBuf);
}

std::wstring TrimString(const std::wstring& src, const WCHAR*trimCharacterList)
{
	const std::wstring::size_type left = src.find_first_not_of(trimCharacterList);	
	const std::wstring::size_type right = src.find_last_not_of(trimCharacterList);

	if (left == std::wstring::npos) {
		if (right == std::wstring::npos) {
			return src;
		}else {
			//"012__"
			return src.substr(0,right+1);
		}
	}else{
		if (right == std::wstring::npos) {
			//"__234"
			return src.substr(left, src.size() - left);
		}else {
			//"__234__"
			return src.substr(left, right - left + 1);
		}
	}
}