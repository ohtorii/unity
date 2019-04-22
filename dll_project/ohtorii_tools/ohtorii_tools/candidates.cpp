#include"stdafx.h"


Candidates::Candidates() {

}

/*bool Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate) {
	m_candidates.emplace_back(source_name, candidate);
	return true;
}*/

bool Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description, const WCHAR*user_data)
{
	m_candidates.emplace_back(source_name, candidate, description, user_data);
	return true;
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

const WCHAR* Candidates::GetUserData(INT_PTR index)const {
	return m_candidates.at(index).m_user_data.c_str();
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

