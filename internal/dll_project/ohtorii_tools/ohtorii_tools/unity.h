#pragma once
#include<vector>
#include<string>
#include<array>
#include"define.h"
#include"sources.h"
#include"file.h"
#include"candidates.h"
#include"refine_search.h"
#include"kinds.h"
#include"inheritance.h"
#include"user_data.h"
#include"static_status.h"
#include"context_status.h"
#include"async_files.h"
#include"recurring_task.h"
#include"hidemaru_functions.h"



class Unity{
public:
	Unity();
	~Unity();

	static std::weak_ptr<Unity> Instance(size_t index= UNITY_USE_CURRENT_INSTANCE);
	static bool PushContext(bool exist_context_then_delete =true);
	static bool PopContext(bool exist_context_then_delete = true);
	static size_t GetCurrentInstanceIndex();
	static bool SerializeStaticContext(const WCHAR*output_filename);
	static bool DeSerializeToStaticContext(const WCHAR*input_filename);
	static bool SerializeStaticStatusContext(const WCHAR*output_filename);
	static bool DeSerializeToStaticStatusContext(const WCHAR*input_filename);
	static void Destroy();
	
	///候補に変更あり
	void ChangeCandidates();
	///候補に変更があるかどうか
	bool HasChangedCandidates();
	///候補に変更なし、かつ、直前の状態を返す
	bool ClearChangedCandidatesAndReturnPrevStatus();

	/// <summary>
    /// is_interactive=tureのソース名をseparatorで連結して取得する。
	/// 秀丸マクロでは、返値の文字列をseparatorで分割して複数のソース名を取得します
	/// </summary>
	/// <param name="separator">セーパレーター</param>
	/// <returns>一時的なポインタを返します</returns>
	const WCHAR* GetInteractiveSourceNames(const WCHAR* separator);

	bool StatusUpdate(const WCHAR*kind_name, const WCHAR*action_name, INT_PTR context_index);

	Sources& 		QuerySources();
	File&			QueryFile();
	Candidates&		QueryCandidates();
	RefineSearch&	QueryRefineSearch();
	Kinds&			QueryKinds();	
	Inheritance&	QueryInheritance();
	UserData&		QueryUserData();
	ContextStatus&	QueryContextStatus();
	StaticStatus&	QueryStaticStatus();
	ASyncFiles&		QueryASyncFiles();
	RecurringTask&	QueryRecurringTask();
	HidemaruFunctions& QueryHidemaruFunctions();

	bool AutoPreviewRegist(const WCHAR*filename);
	/// <summary>
	/// ソースの候補を削除します
	/// </summary>
	/// <param name="source_name"></param>
	/// <returns></returns>
	bool CandidatesClearWithSourceName(const WCHAR* source_name);

	template<class Archive> void serialize(Archive & archive) {
		archive(m_candidates,m_refine_search,m_user_data);
	};

protected:
private:
	Unity(const Unity&);
	Unity &operator=(const Unity&);


	static	std::array<std::shared_ptr<Unity>, UNITY_MAX_CONTEXT_NUM>	m_instances;
	static	size_t					m_current_instance_index;
	bool			m_changed_candidates;
    /// <summary>
    /// is_interactive=tureのソース名をseparatordで連結した名前
    /// </summary>
    std::wstring	m_last_interactive_source_names;

	File				m_file;
	
	/// <summary>
	/// 候補
	/// </summary>
	Candidates			m_candidates;

	/// <summary>
	/// 絞り込み検索
	/// </summary>
	RefineSearch		m_refine_search;
	
	Inheritance			m_inheritance;	
	UserData			m_user_data;
	ASyncFiles			m_async_files;
	ContextStatus		m_context_status;
	
	//
	//staticな情報
	//
	static Sources			m_sources;
	static RecurringTask	m_recurring_task;	
	static StaticStatus		m_static_status;
	///カインドはインスタンス共通で利用するため静的領域とする
	static Kinds			m_kinds;	
	///秀丸エディタの関数
	static HidemaruFunctions	m_hidemaru_functions;

	//std::vector<Candidate>	m_input;
	//HidemaruView				m_hidemaru_view;
	
	
	//bool LoadTextFile(const WCHAR*filename);
	
};
