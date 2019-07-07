#pragma once
#include<string>


class Unity;

class InterfaceSugar {
	friend class Unity;
public:
	static InterfaceSugar& Instance();

	template<class Archive> void serialize(Archive & archive) {
		archive(m_current_source_name);
	};

	InterfaceSugar();
	bool SetCurrenSourceName(const WCHAR*source_name);
	bool ClearCurrentSourceName();
	bool AppendCandidateHeader(const WCHAR*header, const WCHAR*description);
	bool AppendCandidate(const WCHAR*candidate, const WCHAR*description);
	bool AppendUserDataString(const WCHAR* key, const WCHAR* data);
	bool AppendUserDataNumeric(const WCHAR* key, INT_PTR data);
	bool AppendChildCandidate(const WCHAR* candidate, const WCHAR* description);
protected:
	static InterfaceSugar	m_instance;
private:
	std::wstring	m_current_source_name;
	INT_PTR			m_current_candidate_index;
};