#pragma once
#include<string>

/*èÛë‘
*/
class Status {
public:		
	template<class Archive> void serialize(Archive & archive) {
		archive(m_is_start);
	};

	struct IsStart {
		IsStart() {
			m_is_start = false;
		};

		template<class Archive> void serialize(Archive & archive) {
			archive(m_source_name,m_arg);
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

private:
	IsStart		m_is_start;
	//bool	m_is_quit;
};