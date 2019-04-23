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

	///ソース名
	std::wstring			m_source_name;
	///ファイルリストから読み込んだファイル名
	std::wstring			m_text;	
	///説明
	std::wstring			m_description;
	///ユーザーデータ（文字列）
	std::unordered_map<std::wstring, std::wstring>	m_user_data_string;
	///ユーザーデータ（数値）
	std::unordered_map<std::wstring, INT_PTR>		m_user_data_numeric;
	///このテキストが秀丸エディタ側で選択されているかどうか
	bool					m_selected;
	///必ず表示する候補かどうか
	bool					m_fource_show;	
};
/*
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
*/

class Candidates {
public:
	Candidates();
	//bool AppendCandidate(const WCHAR*source_name, const WCHAR*candidate);
	
	/**候補を追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description=_T(""));
	
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

private:
	/*SourceCandidate* AppendIfNotExist(const WCHAR* source_name);
	SourceCandidate* Find(const WCHAR* source_name);
	*/
	//memo: 並び順が重要なのでvector<>を利用
	//std::vector<SourceCandidate>	m_sources;

	//memo: 並び順が重要なのでvector<>を利用
	std::vector<Candidate>	m_candidates;

};