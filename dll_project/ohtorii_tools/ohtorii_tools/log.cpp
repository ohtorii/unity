#include"stdafx.h"


void DebugLog(const WCHAR *fmt, ...)
{
	va_list		ap;
	int			len = 0;
	std::vector<wchar_t>	buffer;

	va_start(ap, fmt);
	len = _vscwprintf(fmt, ap) + 1;//+1 == '\0'
	if (len) {
		buffer.resize(len);
		vswprintf_s(buffer.data(), len, fmt, ap);

		// VisualStudioのデバッグウィンドウには必ず出力する
		OutputDebugString(buffer.data());
	}
	va_end(ap);
}

void DebugLogLastError(DWORD errorcode) {
	LPVOID lpMsgBuf = 0;
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
