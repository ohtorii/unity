#pragma once
#include<string>
#include<stdio.h>

///非同期ファイル
class ASyncFile {
public:
	ASyncFile();
	ASyncFile(const WCHAR*source_name, const WCHAR*filename);
	void Exec(Candidates::ContainerType&dst);


private:
	bool OpenFile();



	///ソース名
	std::wstring			m_source_name;
	///ファイル名
	std::wstring			m_filename;
	FILE*					m_file;
	int						m_mode;
	std::chrono::system_clock::time_point	m_file_read_start_clock;
};

class ASyncFiles {
public:
	ASyncFiles(Unity*instance);
	INT_PTR AppendCandidate(const WCHAR* spuce_name, const WCHAR* filename);
	void Exec();

//	const std::vector<std::shared_ptr<ASyncFile>>&	GetASyncFiles()const { return m_async_files; }
	//std::vector<std::shared_ptr<ASyncFile>>&		GetASyncFiles() { return m_async_files; }

private:
	Unity*					m_instance;
	///非同期ファイル
	std::vector<ASyncFile>	m_async_files;
};