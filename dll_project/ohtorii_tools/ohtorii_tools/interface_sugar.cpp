#include"stdafx.h"



InterfaceSugar InterfaceSugar::m_instance;


InterfaceSugar& InterfaceSugar::Instance() {
	return m_instance;
}

InterfaceSugar::InterfaceSugar() {
	m_current_candidate_index = UNITY_NOT_FOUND_INDEX;
}

bool InterfaceSugar::SetCurrenSourceName(const WCHAR*source_name) {
	try {
		m_current_source_name.assign(source_name);
		return true;
	}catch (std::exception) {
		//pass
	}
	return false;
}

bool InterfaceSugar::ClearCurrentSourceName() {
	try {
		m_current_source_name.clear();
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool InterfaceSugar::AppendCandidateHeader(const WCHAR*header, const WCHAR*description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendCandidateHeader(m_current_source_name.c_str(), header, description);
	m_current_candidate_index = index;
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}

bool InterfaceSugar::AppendCandidate(const WCHAR*candidate, const WCHAR*description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendCandidate(m_current_source_name.c_str(), candidate, description);	
	m_current_candidate_index = index;
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}

bool InterfaceSugar::SetCandidateDisplayText(const WCHAR*display_name) {
	return Unity::Instance().lock()->QueryCandidates().SetDisplayText(m_current_candidate_index, display_name);
}

bool InterfaceSugar::SetCandidateUserDataString(const WCHAR* key, const WCHAR* data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(m_current_candidate_index, key, data);
}

bool InterfaceSugar::SetCandidateUserDataNumeric(const WCHAR* key, INT_PTR data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(m_current_candidate_index, key, data);
}

bool InterfaceSugar::AppendChildCandidate(const WCHAR* candidate, const WCHAR* description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendChildCandidate(m_current_candidate_index,candidate,description);
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}
