#pragma once
#include<string>
#include<vector>

///候補
struct Candidate {
	Candidate();
	Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description);
	/**候補のユーザーデータを設定する（文字列版）
	同一キーがある場合は上書きします。
	return bool	true	成功
				false	失敗
	*/
	bool SetUserData(const WCHAR* key, const WCHAR*data);

	/**候補のユーザーデータを設定する（数値版）
	同一キーがある場合は上書きします。
	return bool	true	成功
				false	失敗
	*/
	bool SetUserData(const WCHAR* key, INT_PTR data);

	const WCHAR*	GetUserData(const WCHAR* key, const WCHAR*	default_data=_T(""));
	INT_PTR			GetUserData(const WCHAR* key, INT_PTR		default_data=0);

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_source_name ,
			m_text,
			m_description,
			m_child,
			m_user_data_string, 
			m_user_data_numeric, 
			m_header,
			m_selectable,
			m_selected);
	};

	struct ChildCandidate {
		ChildCandidate(){};
		ChildCandidate(const WCHAR*text, const WCHAR*description) : m_text(text), m_description(description){};
		template<class Archive> void serialize(Archive & archive) {
			archive(m_text,m_description);
		};
		///候補
		std::wstring			m_text;
		///説明
		std::wstring			m_description;
	};

	///ソース名
	std::wstring			m_source_name;
	///候補のテキスト
	std::wstring			m_text;	
	///説明
	std::wstring			m_description;
	///子候補
	std::vector <ChildCandidate>	m_child;
	///ユーザーデータ（文字列）
	std::unordered_map<std::wstring, std::wstring>	m_user_data_string;
	///ユーザーデータ（数値）
	std::unordered_map<std::wstring, INT_PTR>		m_user_data_numeric;
	///候補のヘッダーかどうか
	bool					m_header;
	///選択可能な候補かどうか
	bool					m_selectable;
	///このテキストが秀丸エディタ側で選択されているかどうか
	bool					m_selected;
};


class Candidates {
public:
	Candidates();
	
	/**ヘッダーを追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidateHeader(const WCHAR*source_name, const WCHAR*header, const WCHAR*description=_T(""));
	
	/**候補を追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description=_T(""));

	/**候補の子供を追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendChildCandidate(INT_PTR candidate_index, const WCHAR*candidate, const WCHAR*description = _T(""));

	/**候補のユーザーデータを設定する（文字列版）
	同一キーがある場合は上書きします。
	return bool	true	成功
				false	失敗
	*/
	bool SetUserData(INT_PTR index, const WCHAR* key, const WCHAR*data);

	/**候補のユーザーデータを設定する（数値版）
	同一キーがある場合は上書きします。
	return bool	true	成功
				false	失敗
	*/
	bool SetUserData(INT_PTR index, const WCHAR* key, INT_PTR data);

	const WCHAR*	GetUserData(INT_PTR index, const WCHAR* key, const WCHAR*	default_data = _T(""));
	INT_PTR			GetUserData(INT_PTR index, const WCHAR* key, INT_PTR		default_data = 0);

	//const std::vector<SourceCandidate>&	GetSources()const { return m_sources; }
	const std::vector<Candidate>&	GetCandidates()const	{ return m_candidates; }
	std::vector<Candidate>&			GetCandidates()			{ return m_candidates; }

	const WCHAR* GetSourceName(INT_PTR index)const;
	const WCHAR* GetText(INT_PTR index)const;
	const WCHAR* GetDescription(INT_PTR index)const;	

	template<class Archive> void serialize(Archive & archive) {
		archive(m_candidates);
	};

private:
	/*SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);
	*/
	//memo: 並び順が重要なのでvector<>を利用
	//std::vector<SourceCandidate>	m_sources;

	//memo: 並び順が重要なのでvector<>を利用
	std::vector<Candidate>	m_candidates;

};