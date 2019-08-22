#include"stdafx.h"

ASyncFile::ASyncFile() {
	m_file = nullptr;
	m_mode = 0;
}

ASyncFile::ASyncFile(const WCHAR*source_name, const WCHAR*filename) :
	m_source_name(source_name),
	m_filename(filename) 
{
	m_file = nullptr;
	m_mode = 0;
}

bool ASyncFile::OpenFile() {
	if (m_file != nullptr) {
		//ファイルはすでに開かれている
		return true;
	}
	m_file = _wfsopen(m_filename.c_str(), _T("rS, ccs=UTF-8"), _SH_DENYNO);
	if ( m_file != nullptr) {
		return true;
	}
	return false;
}

void ASyncFile::Exec(Candidates::ContainerType&dst) {
	TCHAR	line[4096];

	switch (m_mode) {
	case 0:
		if (! OpenFile()) {
			return;
		}
		m_file_read_start_clock = std::chrono::system_clock::now();
		++m_mode;
	case 1:
		if (fgetws(line, _countof(line), m_file) != nullptr) {
			dst.emplace_back(m_source_name.c_str(), line);
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
			if (second < 8) {
				return;
			}
		}
		
		//しばらく待っても行を読み込めないためファイル生成が終了したとみなす。
		++m_mode;
	case 2:
		fclose(m_file);
		m_file = nullptr;
		m_mode = -1;
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

INT_PTR ASyncFiles::AppendCandidate(const WCHAR* source_name, const WCHAR* filename) {
	try {
		m_async_files.emplace_back(source_name, filename);
	}
	catch (std::exception) {
		return UNITY_NOT_FOUND_INDEX;
	}
	return m_async_files.size() - 1;
}

void ASyncFiles::Exec() {
	auto& candidates = m_instance->QueryCandidates().GetCandidates();
	for (auto&file:m_async_files) {
		file.Exec(candidates);
	}
}
