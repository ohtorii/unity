#pragma once
#include<string>

class Unity;


/*状態
*/
class StaticStatus {
public:		
	StaticStatus();

	template<class Archive> void serialize(Archive & archive) {
		archive(m_is_start,
				m_is_quit,
				m_reget_candidate_source_names,
				m_hidemaruhandle_to_focus_at_end_of_process,
				m_target_hidemaruhandle,
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

	void Initialize(INT_PTR target_hidemaru, const WCHAR* working_directory, const WCHAR*root_macro_directory);
	bool Reset(const WCHAR*kind_name,const WCHAR*action_name);

	/*再取得する候補のソース名の個数を得る
	*/
	INT_PTR GetNumberOfSourceNamesForReacquisitionCandidates()const;

	/*再取得する候補のソース名を得る
	*/
	const WCHAR* GetSourceNameForReacquisitionCandidates(INT_PTR index)const;


public:
	bool			m_is_quit;
	std::vector<std::wstring>	m_reget_candidate_source_names;
	INT_PTR			m_hidemaruhandle_to_focus_at_end_of_process;
	INT_PTR			m_target_hidemaruhandle;
	std::wstring	m_current_working_directory;
	std::wstring	m_root_macro_directory;
private:
	IsStart		m_is_start;
};