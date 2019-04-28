#include"stdafx.h"

File::File(){
	
}

File::~File(){
	
}

void File::RegistAfterDelete(const WCHAR*filename){
	
}

bool File::CreateTempFile(std::wstring&out) {
	wchar_t szTempPath[MAX_PATH];
	wchar_t szTempFileName[MAX_PATH];

	out.clear();
	if (GetTempPath(sizeof(szTempPath) / sizeof(szTempPath[0]), szTempPath) == 0) {
		return false;
	}
	if (GetTempFileName(szTempPath, _T("hidemaru_"), 0, szTempFileName) == 0) {
		return false;
	}
	out.assign(szTempFileName);
	return true;
}


// ワイド文字列からマルチバイト文字列へ
static void WToA(std::string&out, const wchar_t* wcs)
{
	const size_t BUFFLEN = 32 * 1024;

	size_t rv = 0;
	std::vector<char> buffer;
	buffer.resize(BUFFLEN);
	setlocale(LC_CTYPE, "ja-JP");
	wcstombs_s(&rv, &buffer[0], BUFFLEN, wcs, _TRUNCATE);
	out.assign(&buffer[0]);

	/*
	size_t n = strlen(buffer);
	n++;
	char* astr = new char[n];
	strcpy_s(astr, n, buffer);
	return astr;*/
}

bool File::WriteToFile(const WCHAR* filename, const WCHAR* string) {

	//std::string mbs;
	//WToA(mbs,string);

	FILE* fp = 0;
	errno_t  error = _wfopen_s(&fp, filename, L"wb");
	if (error != 0) {
		return false;
	}
	if (fp == 0) {
		return false;
	}
	//size_t n = mbs.size();
	//size_t ret = fwrite(mbs.c_str(), sizeof(std::string::value_type), n, fp);

	//BOMを書き込む
	unsigned char bom[] = { 0xff,0xfe };
	fwrite(bom, sizeof(bom[0]), sizeof(bom), fp);
	size_t n = wcslen(string);
	size_t ret = fwrite(string, sizeof(WCHAR), n, fp);
	fclose(fp);
	if (ret < n) {
		return false;
	}
	return true;
}
