#pragma once
#include<string>
#include<stdio.h>
#include<thread>
#include<list>


///非同期ファイル
class ASyncFile {
public:
	ASyncFile();
	ASyncFile(Unity*instance, const WCHAR*source_name, const WCHAR*filename);
	void RequestExit();
	void operator()();

private:
	bool OpenFile();
	void Exec();
	void DoLine(WCHAR*line);
	void Destroy();


	enum StaticStatus{
		FILE_OPEN,
		GET_LINE,
		FILE_CLOSE,
		FINISH,
		//
		INITIALIZED,
	};
	///ソース名
	std::wstring			m_source_name;
	///ファイル名
	std::wstring			m_filename;
	FILE*					m_file;
	StaticStatus						m_status;
	std::chrono::system_clock::time_point	m_file_read_start_clock;
	//Candidates::ContainerType&	m_candidates;
	Unity*					m_instance;
	bool					m_request_exit;
	bool					m_check_bom;
};


class ASyncFiles {
public:
	ASyncFiles(Unity*instance);
	INT_PTR AppendCandidate(const WCHAR* source_name, const WCHAR* filename);
	void Exec();
	void Destroy();

private:
	Unity*					m_instance;

	struct Thread {
		Thread(Unity*instance, const WCHAR*source_name, const WCHAR*filename) :
		m_file(instance, source_name, filename)
		{
		};
		ASyncFile		m_file;
		std::thread		m_thread;
	};
	///非同期ファイル
	std::list<Thread>	m_threads;
};