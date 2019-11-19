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
	auto&			file = Unity::Instance().lock()->QueryFile();
	std::wstring	temp_filename;

	if (!file.CreateTempFile(temp_filename)) {
		return gs_empty;
	}

	const WCHAR*ini_filename = temp_filename.c_str();
	if (!file.WriteToFile(ini_filename, source_ini)) {
		return gs_empty;
	}

	Source dst;
	if (!IniToSource(dst, ini_filename)) {
		return gs_empty;
	}
	auto insert_it = m_sources.insert({ dst.m_name, dst });	
	return const_cast<WCHAR*>(insert_it.first->first.c_str());
}

Source* Sources::FindSource(const WCHAR*source_name) {
	auto it = m_sources.find(source_name);
	if (it == m_sources.end()) {
		return nullptr;
	}
	return &(it->second);
}

bool Sources::MakeSourcePathName(std::wstring&out, const WCHAR*file_name) {
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	const errno_t err = _wsplitpath_s(file_name, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
	if (err != 0) {
		return false;
	}
	
	out.assign(drive);
	out.append(dir);
	out.append(fname);

	//(memo)ファイルパスを小文字で比較する。
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);
	return true;
}

bool Sources::AppendFileNameAndSourceName(const WCHAR*file_name, const WCHAR*source_name) {
	std::wstring path;
	if (!MakeSourcePathName(path, file_name)) {
		return false;
	}

	try {
		m_file_name_to_source_name.emplace(path, source_name);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool Sources::ExistFileName(const WCHAR*file_name)const {
	std::wstring path;
	if (!MakeSourcePathName(path, file_name)) {
		return false;
	}
	auto it = m_file_name_to_source_name.find(path);
	if (it == m_file_name_to_source_name.end()) {
		return false;
	}	
	return true;
}

const WCHAR* Sources::FileNameToSourceName(const WCHAR*file_name)const {
	std::wstring path;
	if (!MakeSourcePathName(path, file_name)) {
		return gs_empty;
	}
	try {
		auto it = m_file_name_to_source_name.find(path);
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
	static std::wstring	result;

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

bool Sources::IniToSource(Source&dst, const WCHAR*ini_filename) {	
	WCHAR buf[16 * 1024];
	
	GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_name.assign(buf);	
	if(dst.m_name.size()==0)
	{
		WCHAR fname[_MAX_FNAME];
		const errno_t err = _wsplitpath_s(ini_filename, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, nullptr, 0);
		if (err != 0) {
			return false;
		}
		dst.m_name.assign(fname);		
	}

	GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_description.assign(buf);

	GetPrivateProfileString(_T("property"), _T("default_kind"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_default_kind.assign(buf);

	GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_default_action.assign(buf);

	GetPrivateProfileString(_T("property"), _T("candidate_type"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_candidate_type.assign(buf);
	
	return true;
}

bool Sources::LoadSourceAll(const WCHAR* root_dir) {
	File::EnumeFileResultContainer file_names;
	if (!Unity::Instance().lock()->QueryFile().EnumeFiles(file_names, root_dir, _T("*.ini"))) {
		return false;
	}

	const auto num = file_names.size();
	Source dst;
	for (size_t i = 0; i < num; ++i) {
		const auto &abs_filename = file_names.at(i).m_abs_filename;
		if (!IniToSource(dst, abs_filename.c_str())) {
			return false;
		}
		m_sources.insert({ dst.m_name, dst });
		if (!AppendFileNameAndSourceName(abs_filename.c_str(), dst.m_name.c_str())) {
			return false;
		}		
	}

	DebugLog(_T("Sources::LoadSourceAll"));
	for (const auto&item:m_file_name_to_source_name) {
		DebugLog(_T("    %s -> %s"), item.first.c_str(), item.second.c_str());
	}
	return true;
}