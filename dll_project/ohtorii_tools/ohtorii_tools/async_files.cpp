#include"stdafx.h"


#define WAITING_SECOND		8
#define SLEEP_MILLISECOND	20



ASyncFile::ASyncFile(Unity*instance, const WCHAR*source_name, const WCHAR*filename) :
	m_instance(instance),
	m_source_name(source_name),
	m_filename(filename) 
{
	m_request_exit = false;
	m_file = nullptr;
	m_status = FILE_OPEN;
}

void ASyncFile::RequestExit() {
	m_request_exit = true;
}


bool ASyncFile::OpenFile() {
	if (m_file != nullptr) {
		//ファイルはすでに開かれている
		return true;
	}
	//(memo) _SH_DENYNO=排他制御なしでファイルを開く
	m_file = _wfsopen(m_filename.c_str(), _T("rS, ccs=UTF-8"), _SH_DENYNO);
	if ( m_file != nullptr) {
		return true;
	}
	return false;
}

void ASyncFile::operator()() {
	while ((m_status!=FINISH) && (m_request_exit==false)) {
		Exec();
	}
}

void ASyncFile::Exec(){
	TCHAR	line[4096];

	switch (m_status) {
	case FILE_OPEN:
		if (! OpenFile()) {
			return;
		}
		m_file_read_start_clock = std::chrono::system_clock::now();
		m_status=GET_LINE;

	case GET_LINE:
		if (fgetws(line, _countof(line), m_file) != nullptr) {
			{
				Candidates::ContainerType&candidates = m_instance->QueryCandidates().GetCandidates();
				Candidates::ContainerType::scoped_lock locker(candidates);
				candidates.emplace_back(m_source_name.c_str(), line);
				m_instance->ChangeCandidates();
			}
			m_file_read_start_clock = std::chrono::system_clock::now();
			return;
		}

		//
		//読み込む行が見つからない場合
		//
		{
			//ファイル内容を作成中の可能性があるため少し待つ。
			auto duration = std::chrono::system_clock::now() - m_file_read_start_clock;
			auto second = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
			if (second < WAITING_SECOND) {
				//少し待つ
				std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLISECOND));
				return;
			}
		}
		
		//しばらく待っても行を読み込めないためファイル生成が終了したとみなす。
		m_status=FILE_CLOSE;

	case FILE_CLOSE:
		fclose(m_file);
		m_file = nullptr;
		m_status = FINISH;

	default:
		//pass
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ASyncFiles
/////////////////////////////////////////////////////////////////////////////
ASyncFiles::ASyncFiles(Unity* instance) {
	m_instance = instance;
}

void ASyncFiles::Destroy() {
	if (m_instance == nullptr) {
		return;
	}
	for (auto &item:m_threads) {
		item.m_file.RequestExit();
	}
	for (auto &item : m_threads) {
		if (item.m_thread.joinable()) {
			item.m_thread.join();
		}
	}
	m_threads.clear();
	m_instance = nullptr;
}

INT_PTR ASyncFiles::AppendCandidate(const WCHAR* source_name, const WCHAR* filename) {
	try {
		m_threads.emplace_back(Thread(m_instance,source_name,filename));
		
		auto&dst = m_threads.back();
		std::thread t(std::ref(dst.m_file));
		dst.m_thread.swap(t);
	}
	catch (std::exception) {
		return reinterpret_cast<INT_PTR>(nullptr);
	}
	return reinterpret_cast<INT_PTR>(m_threads.back().m_thread.native_handle());
}

void ASyncFiles::Exec() {
	//不要になったスレッドを破棄する
	m_threads.remove_if([](Thread&item) {return !item.m_thread.joinable(); });
	
}
