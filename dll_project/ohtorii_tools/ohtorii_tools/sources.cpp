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
Source::Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&default_action, const std::wstring&candidate_type):
	m_name(name),m_description(description),m_default_kind(default_kind), m_default_action(default_action),m_candidate_type(candidate_type)
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
	std::wstring	default_action;
	std::wstring	candidate_type;
	

	{
		auto&			file = Unity::Instance().lock()->QueryFile();
		std::wstring	temp_filename;

		if (!file.CreateTempFile(temp_filename)) {
			return gs_empty;
		}

		const WCHAR*cname = temp_filename.c_str();
		file.RegistAfterDelete(cname);
		if (!file.WriteToFile(cname, source_ini)) {
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

		GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), cname);
		default_action.assign(buf);

		GetPrivateProfileString(_T("property"), _T("candidate_type"), _T(""), buf, _countof(buf), cname);
		candidate_type.assign(buf);
	}

	size_t old_size = m_sources.size();
	auto insert_it = m_sources.insert({ name, Source(name, description, default_kind, default_action, candidate_type) });
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

bool Sources::AppendFileNameAndSourceName(const WCHAR*file_name, const WCHAR*source_name) {
	try {
		m_file_name_to_source_name.emplace(file_name, source_name);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Sources::ExistFileName(const WCHAR*file_name)const {
	auto it = m_file_name_to_source_name.find(file_name);
	if (it == m_file_name_to_source_name.end()) {
		return false;
	}	
	return true;
}

const WCHAR* Sources::FileNameToSourceName(const WCHAR*file_name)const {
	try {
		auto it = m_file_name_to_source_name.find(file_name);
		if (it == m_file_name_to_source_name.end()) {
			return gs_empty;
		}
		return it->second.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}

const WCHAR* Sources::SourceNameToFileName(const WCHAR*source_name)const {
	for(const auto& var :m_file_name_to_source_name){
		if (var.second == source_name) {
			return var.first.c_str();
		}
	}
	return gs_empty;
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