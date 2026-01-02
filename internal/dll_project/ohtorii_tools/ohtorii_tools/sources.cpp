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
Source::Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&default_action, const std::wstring&candidate_type, bool is_interactive)
	:
	m_name(name),
	m_description(description),
	m_default_kind(default_kind), 
	m_default_action(default_action),
	m_candidate_type(candidate_type), 
	m_is_interactive(is_interactive)
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

std::wstring Sources::Create(const WCHAR*source_name, const WCHAR* ini_image){
	auto&			file = Unity::Instance().lock()->QueryFile();
	std::wstring	temp_filename;

	if (!file.CreateTempFile(temp_filename)) {
		return gs_empty;
	}

	const WCHAR*ini_filename = temp_filename.c_str();
	if (!file.WriteToFile(ini_filename, ini_image)) {
		return gs_empty;
	}

	Source dst;
	if (!IniToSource(dst, source_name, ini_filename)) {
		return gs_empty;
	}
	auto insert_it = m_sources.insert({ dst.m_name, dst });	
	return insert_it.first->first;
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
	
	const errno_t err = _wsplitpath_s(file_name, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, nullptr, 0);
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
		if (var.second.compare(source_name)==0) {
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

bool Sources::IniToSource(Source&dst, const WCHAR*source_name, const WCHAR*ini_filename) {
	std::vector<WCHAR> buf;
	const DWORD size = 8 * 1000;
	buf.resize(size, 0);
	auto* data = buf.data();

	if (GetPrivateProfileString(_T("property"), _T("name"), L"", data, size, ini_filename)) {
		dst.m_name.assign(data);
	}
	else {
        dst.m_name.assign(source_name);
	}

	GetPrivateProfileString(_T("property"), _T("description"), _T(""), data, size, ini_filename);
	dst.m_description.assign(data);

	GetPrivateProfileString(_T("property"), _T("default_kind"), _T(""), data, size, ini_filename);
	dst.m_default_kind.assign(data);

	GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), data, size, ini_filename);
	dst.m_default_action.assign(data);

	GetPrivateProfileString(_T("property"), _T("candidate_type"), _T(""), data, size, ini_filename);
	dst.m_candidate_type.assign(data);
	
	GetPrivateProfileString(_T("property"), _T("is_interactive"), _T(""), data, size, ini_filename);
	if (_wcsnicmp(data, L"true", size) == 0) {
		//明示的にtrueを指定した状態
		dst.m_is_interactive = true;
	}
	else {
		//明示的にfalseを指定、または、空白、誤字脱字の状態
		dst.m_is_interactive = false;
	}
	
	return true;
}

void Sources::SetRootPath(const WCHAR* root_dir) {
    m_root_path.assign(root_dir);
	// \で終端させる
	if (!m_root_path.empty()) {
		const WCHAR last_char = m_root_path.back();
		if (last_char != L'\\' && last_char != L'/') {
			m_root_path.push_back(L'\\');
		}
    }
}

const std::wstring& Sources::GetRootPath() const { 
	return m_root_path; 
}

bool Sources::LoadSourceAll(const WCHAR* root_dir) {
	File::EnumeFileResultContainer file_names;
	if (!Unity::Instance().lock()->QueryFile().EnumeFiles(file_names, root_dir, _T("*.ini"))) {
		return false;
	}

	WCHAR fname[_MAX_FNAME];
	const auto num = file_names.size();
	Source dst;
	for (size_t i = 0; i < num; ++i) {
		const auto &abs_filename = file_names.at(i).m_abs_filename;
		errno_t err = _wsplitpath_s(
			abs_filename.c_str(),
			nullptr, 0,
			nullptr, 0,
			fname, _countof(fname),
			nullptr, 0
		);
		if (err != 0) {
			return false;
		}

		if (!IniToSource(dst, fname, abs_filename.c_str())) {
			return false;
		}
		m_sources.insert({ dst.m_name, dst });
		if (!AppendFileNameAndSourceName(abs_filename.c_str(), dst.m_name.c_str())) {
			return false;
		}		
	}

	DebugLog(_T("Sources::LoadSourceAll"));
	for (const auto&item:m_file_name_to_source_name) {
		DebugLog(_T("    %ls -> %ls"), item.first.c_str(), item.second.c_str());
	}
	return true;
}