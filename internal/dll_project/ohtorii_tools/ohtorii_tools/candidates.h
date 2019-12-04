#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include"parallel_container.h"


///候補
struct Candidate {
	Candidate();
	Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description=_T(""));
	Candidate(const WCHAR*source_name, const WCHAR*prefix, const WCHAR*text, const WCHAR*postfix, const WCHAR*description = _T(""));

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
			m_action,
			m_prefix,
			m_text,
			m_postfix,
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

	///アクションの情報
	struct Action{
		Action() {
			m_column = 1;
			m_line = 1;
			m_process_id= UNITY_INVALID_PROCESS_ID;
			m_window_handle = UNITY_HIDEMARU_NULL_HANDLE;
		};

		template<class Archive> void serialize(Archive & archive) {
			archive(
				m_column, 
				m_line,
				m_directory_name,
				m_file_name,
				m_command,
				m_process_id,
				m_window_handle);
		};
		///アクションのカラム位置
		INT_PTR				m_column;
		///アクションの行番号
		INT_PTR				m_line;
		///アクションのディレクトリ名
		std::wstring			m_directory_name;
		///アクションのファイル名
		std::wstring			m_file_name;
		///アクションのコマンド
		std::wstring		m_command;
		///アクションのプロセスID
		INT_PTR				m_process_id;
		///アクションのウインドウハンドル
		INT_PTR				m_window_handle;
	};
	Action				m_action;
	///ソース名
	std::wstring			m_source_name;	
	///候補のテキスト（prefix）
	std::wstring			m_prefix;
	///候補のテキスト
	std::wstring			m_text;	
	///候補のテキスト（postfix）
	std::wstring			m_postfix;
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
	typedef ParallelVector<Candidate>	ContainerType;

	Candidates();
	
	/**ヘッダーを追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidateHeader(const WCHAR*source_name, const WCHAR*header, const WCHAR*description=_T(""));
	
	/**候補を追加する
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description=_T(""));

	/**候補を追加する(prefix/postfix指定)
	return 候補へのインデックス
	*/
	INT_PTR AppendCandidateFix(const WCHAR*source_name, const WCHAR*pretfix, const WCHAR*candidate, const WCHAR*postfix, const WCHAR*description = _T(""));


	/**アクションのdirectory名を設定する
	return bool	true	成功
				false	失敗
	*/
	bool SetActionDirectoryName(INT_PTR index, const WCHAR* directory_name);

	/**アクションのファイル名を設定する
	return bool	true	成功
				false	失敗
	*/
	bool SetActionFileName(INT_PTR index, const WCHAR* filename);

	/**アクションのカラム位置を設定する
	*/
	bool SetActionColumn(INT_PTR index, INT_PTR column);

	/**アクションの行番号を設定する
	*/
	bool SetActionLine(INT_PTR index, INT_PTR line);

	/**アクションのコマンドを設定する
	*/
	bool SetActionCommand(INT_PTR index, const WCHAR* command);

	/**アクションのプロセスIDを設定する
	*/
	bool SetActionProcessId(INT_PTR index, INT_PTR process_id);

	/**アクションのウインドウハンドルを設定する
	*/
	bool SetActionWindowHandle(INT_PTR index, INT_PTR window_handle);

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
	const ContainerType&	GetCandidates()const	{ return m_candidates; }
	ContainerType&			GetCandidates()			{ return m_candidates; }

	const WCHAR* GetSourceName(INT_PTR index)const;
	const WCHAR* GetText(INT_PTR index)const;
	const WCHAR* GetActionDirectoryName(INT_PTR index)const;
	const WCHAR* GetActionFileName(INT_PTR index)const;
	INT_PTR		GetActionColumn(INT_PTR index)const;
	INT_PTR		GetActionLine(INT_PTR index)const;
	const WCHAR* GetActionCommand(INT_PTR index)const;
	INT_PTR		GetActionProcessId(INT_PTR index)const;
	INT_PTR		GetActionWindowHandle(INT_PTR index)const;
	const WCHAR* GetDescription(INT_PTR index)const;	

	/*指定ソース名の候補を削除する
	sourcename	ソース名
	return	bool	true	操作に成功
					false	操作に失敗
	*/
	bool ClearWithSourceName(const WCHAR*sourcename);

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
	mutable ContainerType	m_candidates;

};