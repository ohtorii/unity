#include"stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//Candidate
/////////////////////////////////////////////////////////////////////////////
Candidate::Candidate() {
	m_selected = false;
	m_fource_show = false;
}

Candidate::Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description) :
	m_source_name(source_name),
	m_text(text),
	m_description(description)
{
	m_selected = false;
	m_fource_show = false;
}

bool Candidate::SetUserData(const WCHAR* key, const WCHAR*data) {
	return m_user_data_string.emplace(key, data).second;
}

bool Candidate::SetUserData(const WCHAR* key, INT_PTR data) {
	return m_user_data_numeric.emplace(key, data).second;
}

const WCHAR*	Candidate::GetUserData(const WCHAR* key, const WCHAR*	default_data) {
	const auto it = m_user_data_string.find(key);
	if (it == m_user_data_string.end()) {
		return default_data;
	}
	return it->second.c_str();
}

INT_PTR			Candidate::GetUserData(const WCHAR* key, INT_PTR		default_data) {
	const auto it = m_user_data_numeric.find(key);
	if (it == m_user_data_numeric.end()) {
		return default_data;
	}
	return it->second;
}


/////////////////////////////////////////////////////////////////////////////
//Candidates
/////////////////////////////////////////////////////////////////////////////
Candidates::Candidates() {

}

/*bool Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate) {
	m_candidates.emplace_back(source_name, candidate);
	return true;
}*/

INT_PTR Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description)
{
	m_candidates.emplace_back(source_name, candidate, description);
	return m_candidates.size() - 1;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, const WCHAR*data) {
	try {
		return m_candidates.at(index).SetUserData(key, data);
	}catch (std::range_error) {
		//pass
	}
	return false;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, INT_PTR data) {
	try {
		return m_candidates.at(index).SetUserData(key, data);
	}catch (std::range_error) {
		//pass
	}
	return false;
}

const WCHAR*	Candidates::GetUserData(INT_PTR index, const WCHAR* key, const WCHAR*	default_data) {
	try {
		return m_candidates.at(index).GetUserData(key, default_data);
	}
	catch (std::range_error) {
		//pass
	}
	return default_data;
}

INT_PTR			Candidates::GetUserData(INT_PTR index, const WCHAR* key, INT_PTR		default_data) {
	try {
		return m_candidates.at(index).GetUserData(key, default_data);
	}
	catch (std::range_error) {
		//pass
	}
	return default_data;
}

const WCHAR* Candidates::GetSourceName(INT_PTR index)const{
	return m_candidates.at(index).m_source_name.c_str();
}

const WCHAR* Candidates::GetText(INT_PTR index)const {
	return m_candidates.at(index).m_text.c_str();
}

const WCHAR* Candidates::GetDescription(INT_PTR index)const {
	return m_candidates.at(index).m_description.c_str();
}

/*
SourceCandidate* Candidates::AppendIfNotExist(const WCHAR* source_name) {
	for (auto& source : m_sources)
	{
		if (source.m_source_name.compare(source_name) == 0) {
			return &source;
		}
	}
	m_sources.emplace_back(source_name);
	return &m_sources.back();
}

SourceCandidate* Candidates::Find(const WCHAR* source_name) {
	for (auto& source : m_sources)
	{
		if (source.m_source_name.compare(source_name) == 0) {
			return &source;
		}
	}
	return nullptr;
}
*/

