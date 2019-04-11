#pragma once


///候補
struct Candidate {
	Candidate() {
		m_selected = false;
		m_fource_show = false;
	};

	Candidate(const WCHAR*text) : m_text(text) {
		m_selected = false;
		m_fource_show = false;
	};
	Candidate(const WCHAR*text, const WCHAR*user_data) : m_text(text),m_user_data(user_data) {
		m_selected = false;
		m_fource_show = false;
	};
	///ファイルリストから読み込んだファイル名
	std::wstring			m_text;
	///ユーザーデータ
	std::wstring			m_user_data;
	///このテキストが秀丸エディタ側で選択されているかどうか
	bool					m_selected;
	///必ず表示する候補かどうか
	bool					m_fource_show;
};

struct SourceCandidate{
	SourceCandidate() {
	}
	SourceCandidate(const WCHAR*source_name) : 
		m_source_name(source_name)
	{

	}
	std::wstring			m_source_name;
	std::vector<Candidate>	m_candidates;
};


class Candidates {
public:
	Candidates();
	bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate);
	bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*user_data);
	
private:
	SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);

	//memo: 並び順が大事なのでvector<>を利用
	std::vector<SourceCandidate>	m_sources;
};