#pragma once
#include<string>

class Unity;


/*状態
*/
class Status {
public:		
	Status();

	template<class Archive> void serialize(Archive & archive) {
		archive(m_is_start,
				m_is_quit,
				m_hidemaruhandle_to_focus_at_end_of_process,
				m_target_hidemaruhandle,
				m_current_working_directory,
				m_root_macro_directory
		);
	};

	struct IsStart {
		IsStart() {
			m_is_start = false;
		};

		template<class Archive> void serialize(Archive & archive) {
			archive(
				m_is_start,
				m_source_name,
				m_arg);
		};
		
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

	bool Reset(const WCHAR*kind_name,const WCHAR*action_name);

public:
	bool			m_is_quit;
	INT_PTR			m_hidemaruhandle_to_focus_at_end_of_process;
	INT_PTR			m_target_hidemaruhandle;
	std::wstring	m_current_working_directory;
	std::wstring	m_root_macro_directory;
private:
	IsStart		m_is_start;
};