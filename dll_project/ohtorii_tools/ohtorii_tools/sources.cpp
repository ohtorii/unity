#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };


///////////////////////////////////////////////////////////////////////////////
//	Source
///////////////////////////////////////////////////////////////////////////////
Source::Source(){
	
}
Source::Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&candidate_type):
	m_name(name),m_description(description),m_default_kind(default_kind),m_candidate_type(candidate_type)
{

}

Source::~Source(){
	
}
	


///////////////////////////////////////////////////////////////////////////////
//	Sources
///////////////////////////////////////////////////////////////////////////////
Sources::Sources(){
	
}

Sources::~Sources(){
	
}

WCHAR* Sources::Create(const WCHAR* source_ini){
	static std::wstring	name;	//staticについて: 秀丸エディタへ文字列を返すため静的なメモリ領域とする
	std::wstring	description;
	std::wstring	default_kind;
	std::wstring	candidate_type;
	

	{
		File*			file = Unity::Instance()->QueryFile();
		std::wstring	temp_filename;

		if (!file->CreateTempFile(temp_filename)) {
			return gs_empty;
		}

		const WCHAR*cname = temp_filename.c_str();
		file->AddAfterDelete(cname);
		if (!file->WriteToFile(cname, source_ini)) {
			return gs_empty;
		}

		WCHAR buf[1024];
		GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), cname);
		name.assign(buf);
		if (name.size() == 0) {
			return gs_empty;
		}
			
		GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), cname);
		description.assign(buf);

		GetPrivateProfileString(_T("property"), _T("default_kind"), _T(""), buf, _countof(buf), cname);
		default_kind.assign(buf);

		GetPrivateProfileString(_T("property"), _T("candidate_type"), _T(""), buf, _countof(buf), cname);
		candidate_type.assign(buf);
	}

	size_t old_size = m_sources.size();
	auto insert_it = m_sources.insert({ name, Source(name, description, default_kind, candidate_type) });
	if (old_size < m_sources.size()) {
		return (WCHAR*)name.c_str();
	}
	return gs_empty;
}

Source* Sources::FindSource(const WCHAR*source_name) {
	auto it = m_sources.find(source_name);
	if (it == m_sources.end()) {
		return nullptr;
	}
	return &(it->second);
}

/*
const WCHAR* Sources::GetCandidateType(const WCHAR*source_name) {
	auto it=m_sources.find(source_name);
	if (it == m_sources.end()) {
		return gs_empty;
	}
	return it->second.m_candidate_type.c_str();
}*/

bool Sources::Exist(const WCHAR*source_name)const {
	return m_sources.end() != m_sources.find(source_name);
}