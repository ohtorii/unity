/*ファイル操作
*/
#include "stdafx.h"

// ワイド文字列からマルチバイト文字列へ
static void WToA(std::string&out, const wchar_t* wcs)
{
	const size_t BUFFLEN=32*1024;

	size_t rv=0;
	char buffer[BUFFLEN];
	setlocale(LC_CTYPE, "ja-JP");
	wcstombs_s(&rv, buffer, BUFFLEN, wcs, _TRUNCATE);
	out.assign(buffer);
	
	/*
	size_t n = strlen(buffer);
	n++;
	char* astr = new char[n];
	strcpy_s(astr, n, buffer);
	return astr;*/
}

extern "C" INT_PTR WriteToFile(WCHAR* filename, WCHAR* string) {
	
	std::string mbs;
	WToA(mbs,string);

	FILE* fp=0;
	errno_t  error=_wfopen_s(&fp,filename,L"wb");
	if(error != 0){
		return false;
	}
	size_t n = mbs.size();
	size_t ret = fwrite(mbs.c_str(), sizeof(std::string::value_type), n, fp);
	//size_t n=wcslen(string);
	//size_t ret=fwrite(string,sizeof(WCHAR),n,fp);
	fclose(fp);
	if(ret<n){
		return false;
	}
	return true;
}
