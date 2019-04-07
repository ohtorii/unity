/*ファイル操作
*/
#include "stdafx.h"

extern "C" INT_PTR WriteToFile(WCHAR* filename, WCHAR* string) {
	FILE* fp=0;
	errno_t  error=_wfopen_s(&fp,filename,L"wb");
	if(error != 0){
		return false;
	}
	size_t n=wcslen(string);
	size_t ret=fwrite(string,sizeof(WCHAR),n,fp);
	fclose(fp);
	if(ret<n){
		return false;
	}
	return true;
}
