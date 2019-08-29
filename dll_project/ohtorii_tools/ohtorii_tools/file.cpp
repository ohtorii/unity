#include"stdafx.h"


std::vector<std::wstring>	File::m_after_delete;


File::File(){
	
}

File::~File(){
}

void File::Destroy() {
	DebugLog(_T("File::Destroy() Start. m_after_delete.size()=%d"), m_after_delete.size());
	for (const auto&filename:m_after_delete) {
		DebugLog(_T("\tDelete -> %s"), filename.c_str());
		DeleteFile(filename.c_str());
	}
	m_after_delete.clear();
	DebugLog(_T("File::Destroy() Finish."));
}

bool File::RegistAfterDelete(const WCHAR*filename){
	try {
		m_after_delete.push_back(filename);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool File::CreateTempFile(std::wstring&out) {
	wchar_t szTempPath[MAX_PATH];
	wchar_t szTempFileName[MAX_PATH];

	out.clear();
	if (GetTempPath(sizeof(szTempPath) / sizeof(szTempPath[0]), szTempPath) == 0) {
		return false;
	}
	if (GetTempFileName(szTempPath, _T("hut"), 0, szTempFileName) == 0) {
		return false;
	}
	out.assign(szTempFileName);
	return true;
}

bool File::WriteToFile(const WCHAR* filename, const WCHAR* string) {
	FILE* fp = 0;
	errno_t  error = _wfopen_s(&fp, filename, L"wb");
	if (error != 0) {
		return false;
	}
	if (fp == 0) {
		return false;
	}

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
