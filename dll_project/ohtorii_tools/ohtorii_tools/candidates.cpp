#include"stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//Candidate
/////////////////////////////////////////////////////////////////////////////
Candidate::Candidate() {
	m_header = false;
	m_selectable = true;
	m_selected = false;
}

Candidate::Candidate(const WCHAR*source_name, const WCHAR*text, const WCHAR*description) :
	m_source_name(source_name),
	m_text(text),
	m_description(description)
{
	m_header=false;
	m_selectable=true;
	m_selected = false;
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

INT_PTR Candidates::AppendCandidateHeader(const WCHAR*source_name, const WCHAR*header, const WCHAR*description){
	m_candidates.emplace_back(source_name, header, description);
	auto &dst=m_candidates.back();
	dst.m_header=true;
	dst.m_selectable=false;
	return m_candidates.size() - 1;
}

INT_PTR Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description)
{
	m_candidates.emplace_back(source_name, candidate, description);
	return m_candidates.size() - 1;
}

INT_PTR Candidates::AppendChildCandidate(INT_PTR candidate_index, const WCHAR*candidate, const WCHAR*description) {
	try{
		auto &child = m_candidates.at(candidate_index).m_child;
		child.emplace_back(candidate, description);
		return child.size() - 1;
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

bool Candidates::SetActionDirectoryName(INT_PTR index, const WCHAR* directory_name) {
	try {
		m_candidates.at(index).m_action.m_directory_name.assign(directory_name);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Candidates::SetActionFileName(INT_PTR index, const WCHAR* filename) {
	try {
		m_candidates.at(index).m_action.m_file_name.assign(filename);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Candidates::SetActionColumn(INT_PTR index, INT_PTR column) {
	try {
		m_candidates.at(index).m_action.m_column = column;
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Candidates::SetActionLine(INT_PTR index, INT_PTR line) {
	try {
		m_candidates.at(index).m_action.m_line = line;
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, const WCHAR*data) {
	try {
		return m_candidates.at(index).SetUserData(key, data);
	}catch (std::exception) {
		//pass
	}
	return false;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, INT_PTR data) {
	try {
		return m_candidates.at(index).SetUserData(key, data);
	}catch (std::exception) {
		//pass
	}
	return false;
}

const WCHAR*	Candidates::GetUserData(INT_PTR index, const WCHAR* key, const WCHAR*	default_data) {
	try {
		return m_candidates.at(index).GetUserData(key, default_data);
	}
	catch (std::exception) {
		//pass
	}
	return default_data;
}

INT_PTR			Candidates::GetUserData(INT_PTR index, const WCHAR* key, INT_PTR		default_data) {
	try {
		return m_candidates.at(index).GetUserData(key, default_data);
	}
	catch (std::exception) {
		//pass
	}
	return default_data;
}

const WCHAR* Candidates::GetSourceName(INT_PTR index)const{
	try {
		return m_candidates.at(index).m_source_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}

const WCHAR* Candidates::GetText(INT_PTR index)const {
	try{
		return m_candidates.at(index).m_text.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}

const WCHAR* Candidates::GetActionDirectoryName(INT_PTR index)const {
	try {
		return m_candidates.at(index).m_action.m_directory_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}

const WCHAR* Candidates::GetActionFileName(INT_PTR index)const {
	try {
		return m_candidates.at(index).m_action.m_file_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}

INT_PTR		Candidates::GetActionColumn(INT_PTR index)const {
	try {
		return m_candidates.at(index).m_action.m_column;
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}
INT_PTR		Candidates::GetActionLine(INT_PTR index)const {
	try {
		return m_candidates.at(index).m_action.m_line;
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

const WCHAR* Candidates::GetDescription(INT_PTR index)const {
	try{
		return m_candidates.at(index).m_description.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
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

