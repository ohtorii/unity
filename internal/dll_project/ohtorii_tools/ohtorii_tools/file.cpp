#include"stdafx.h"


std::deque<File::DeleteFile_>	File::m_after_delete;

/////////////////////////////////////////////////////////////////////////////
//DeleteFile_
/////////////////////////////////////////////////////////////////////////////
File::DeleteFile_::DeleteFile_() {
	m_deleting = false;
}

File::DeleteFile_::DeleteFile_(const std::wstring&filename) :m_filename(filename) {
	m_deleting = false;
}

void File::DeleteFile_::DeleteFile() {
	if (m_deleting) {
		return;
	}
	DebugLog(_T("DeleteFile -> %s"), m_filename.c_str());
	m_deleting = true;
	if (::DeleteFile(m_filename.c_str()) == 0) {
		DebugLog(_T("  -> Fail."));
		DebugLogLastError(GetLastError());
	}
	else {
		DebugLog(_T("  -> Success."));
	}
}

void File::DeleteFile_::DeleteFileAsThread() { 
	if (m_deleting) {
		return;
	}
	DebugLog(_T("DeleteFile -> %s"), m_filename.c_str());
	m_deleting = true;
	m_thread= std::thread(::DeleteFile, m_filename.c_str());
}

void File::DeleteFile_::Join() {
	if (!m_deleting) {
		return;
	}
	if (! m_thread.joinable()) {
		return;
	}
	m_thread.join();
}


/////////////////////////////////////////////////////////////////////////////
//File
/////////////////////////////////////////////////////////////////////////////
File::File(){
	
}

File::~File(){
}

void File::StartDestroy() {
	for (auto&item:m_after_delete) {
		item.DeleteFile();
	}
}

void File::JoinDestroy() {
	for (auto&item : m_after_delete) {
		item.Join();
	}
	m_after_delete.clear();
}

bool File::RegistAfterDelete(const WCHAR*filename){
	try {
		m_after_delete.emplace_back(filename);
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

bool File::EnumeFiles(EnumeFileResultContainer&out, const WCHAR*directory, const WCHAR* extension) {
	std::wstring	file_pattern;
	file_pattern.append(directory);
	file_pattern.append(extension);
	
	WIN32_FIND_DATA win32fd;
	memset(&win32fd,0,sizeof(win32fd));

	HANDLE hFind = FindFirstFile(file_pattern.c_str(), &win32fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	EnumeFileResultContainer::value_type	value;
	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//pass
		}
		else {
			value.m_abs_filename.assign(directory);
			value.m_abs_filename.append(win32fd.cFileName);
			//value.m_filename.assign(win32fd.cFileName);
			out.emplace_back(value);
		}
	} while (FindNextFile(hFind, &win32fd));
	FindClose(hFind);

	return true;
}