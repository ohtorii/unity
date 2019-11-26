#pragma once
#include<string>

class Unity;


/*静的な状態
*/
class StaticStatus {
public:		
	StaticStatus();

	template<class Archive> void serialize(Archive & archive) {
		archive(m_is_start,
				m_is_quit,
				m_callback,
				m_target_hidemaruhandle,
				m_search_hidemaru_handle,
				m_current_working_directory,
				m_root_macro_directory
		);
	};

	struct IsStart {		
		template<class Archive> void serialize(Archive & archive) {
			archive(
				m_is_start,
				m_source_name,
				m_arg);
		};
		
		IsStart();
		bool Set(const WCHAR*source_name, const WCHAR*arg);
		bool Clear();
		bool SetEnable(bool is_start);
		bool Enable()const;

		bool			m_is_start;
		std::wstring	m_source_name;
		std::wstring	m_arg;
		
	};
	IsStart&GetIsStart();
	const IsStart&GetIsStart() const;

	void Initialize(INT_PTR target_hidemaru, INT_PTR search_hidemaru_handle, const WCHAR* working_directory, const WCHAR*root_macro_directory);
	void UpdateStatus(INT_PTR kind_index, INT_PTR action_index,INT_PTR context_index);
	/*再取得する候補のソース名の個数を得る
	*/
	INT_PTR GetNumberOfSourceNamesForReacquisitionCandidates()const;

	/*再取得する候補のソース名を得る
	*/
	const WCHAR* GetSourceNameForReacquisitionCandidates(INT_PTR index)const;
	/*再取得する候補のソース名をクリアする
	*/
	void ClearSourceNamesForReacquisitionCandidates();


	class CallBack{
	public:
		template<class Archive> void serialize(Archive & archive) {
			archive(m_macro_filename, m_label_name, m_args);
		};

		void Clear() {
			m_macro_filename.clear();
			m_label_name.clear();
			m_args.clear();
		};
		std::wstring	m_macro_filename;
		std::wstring	m_label_name;
		std::wstring	m_args;
	};
	bool SetCallBack(const WCHAR*macro_filename, const WCHAR* label_name, const WCHAR* args);


public:
	bool			m_is_quit;	
	CallBack		m_callback;
	INT_PTR			m_target_hidemaruhandle;
	INT_PTR			m_search_hidemaru_handle;
	std::wstring	m_current_working_directory;
	std::wstring	m_root_macro_directory;

private:
	IsStart		m_is_start;
	std::vector<std::wstring>	m_reget_candidate_source_names;
};