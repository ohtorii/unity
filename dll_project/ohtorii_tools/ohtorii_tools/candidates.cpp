#include"stdafx.h"


Candidates::Candidates() {

}

bool Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate) {
	SourceCandidate* dst = AppendIfNotExist(source_name);
	dst->m_candidates.emplace_back(candidate);
	return true;
}

bool Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*user_data) {
	SourceCandidate* dst = AppendIfNotExist(source_name);
	dst->m_candidates.emplace_back(candidate,user_data);
	return true;
}

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