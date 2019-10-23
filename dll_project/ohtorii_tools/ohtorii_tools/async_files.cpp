#include"stdafx.h"


#define WAITING_SECOND		8
#define SLEEP_MILLISECOND	20


static size_t SkipUTF16Bom(const WCHAR str) {
	if (str == 0xFEFF) {
		return sizeof(WCHAR);
	}
	if (str == 0xFFFE) {
		return sizeof(WCHAR);
	}
	return 0;
}


ASyncFile::ASyncFile() {
	m_file=nullptr;
	m_instance=nullptr;
	m_request_exit=false;
	m_check_bom=false;
	m_status = INITIALIZED;
}

ASyncFile::ASyncFile(Unity*instance, const WCHAR*source_name, const WCHAR*filename) :
	m_instance(instance),
	m_source_name(source_name),
	m_filename(filename),
	m_check_bom(true)
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
	//todo _wfopen_s関数へ移行する
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
	Destroy();
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
			size_t skip_byte = 0;
			if (m_check_bom) {
				skip_byte = SkipUTF16Bom(line[0]);
				m_check_bom = false;
			}
			auto*top = reinterpret_cast<WCHAR*>(reinterpret_cast<char*>(&line[0]) + skip_byte);
			DoLine(top);
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
		
		//少し待っても行を読み込めないためファイル生成が終了したとみなす。
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

void ASyncFile::DoLine(WCHAR*line) {
	std::vector<std::wstring> tokens;
	Tokenize(tokens, line, _T("\t"));

	const auto num = tokens.size();
	if (num == 0) {
		return;
	}

	std::wstring							candidate_text(TrimString(tokens.at(0), _T("\n\r")));
	Candidates::ContainerType&				candidates = m_instance->QueryCandidates().GetCandidates();
	Candidates::ContainerType::scoped_lock	locker(candidates);
	{
		candidates.emplace_back(m_source_name.c_str(), candidate_text.c_str());

		if (3 <= num) {
			const auto	&action_name= tokens.at(1);
			auto		action_text	= TrimString(tokens.at(2), _T("\n\r"));
			auto		&candidate	= candidates.back();
			auto		&action		= candidate.m_action;

			if (action_name == _T("@action_directory")) {
				action.m_directory_name.append(action_text);
			}
			else if (action_name == _T("@action_filename")) {
				action.m_file_name.append(action_text);
			}else if (action_name == _T("@description")) {
				candidate.m_description.assign(action_text);
			}else{
				//pass
			}
		}
	}
	m_instance->ChangeCandidates();
}

void ASyncFile::Destroy() {
	m_status = FINISH;
	if (m_file != nullptr) {
		fclose(m_file);
		m_file = nullptr;
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
