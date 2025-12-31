#include"stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//Candidate
/////////////////////////////////////////////////////////////////////////////
static void ReplaceAll(std::wstring &inout, const std::wstring&to, const std::wstring&from) {
	const auto	from_size	= from.size();
	const auto	to_size		= to.size();
	auto		pos			= inout.find(from);
	while (pos != std::wstring::npos) {
		inout.replace(pos, from_size, to);
		pos = inout.find(from, pos + to_size);
	}
}

static void NormalizeText(std::wstring&inout){
	//Trim
	inout.erase(std::remove(inout.begin(), inout.end(), _T('\n')), inout.end());
	inout.erase(std::remove(inout.begin(), inout.end(), _T('\r')), inout.end());

	//"    " <- "\t"
	ReplaceAll(inout,_T("    "), _T("\t"));
}

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
	NormalizeText(m_text);
	m_header=false;
	m_selectable=true;
	m_selected = false;
}

Candidate::Candidate(const WCHAR*source_name, const WCHAR*prefix, const WCHAR*text, const WCHAR*postfix, const WCHAR*description):
	m_source_name(source_name),
	m_prefix(prefix),
	m_text(text),
	m_postfix(postfix),
	m_description(description)
{
	NormalizeText(m_text);
	m_header = false;
	m_selectable = true;
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
	INT_PTR result=0;
	ContainerType::scoped_lock locker(m_candidates);
	{
		m_candidates.emplace_back(source_name, header, description);
		auto &dst = m_candidates.back();
		dst.m_header = true;
		dst.m_selectable = false;
		result=m_candidates.size() - 1;
	}
	return result;
}

INT_PTR Candidates::AppendCandidate(const WCHAR*source_name, const WCHAR*candidate, const WCHAR*description)
{
	INT_PTR result=0;
	ContainerType::scoped_lock locker(m_candidates);
	{
		m_candidates.emplace_back(source_name, candidate, description);
		result=m_candidates.size() - 1;
	}
	return result;
}

INT_PTR Candidates::AppendCandidateFix(const WCHAR*source_name, const WCHAR*prefix, const WCHAR*candidate, const WCHAR*postfix, const WCHAR*description)
{
	INT_PTR result = 0;
	ContainerType::scoped_lock locker(m_candidates);
	{
		m_candidates.emplace_back(source_name, prefix, candidate, postfix, description);
		result = m_candidates.size() - 1;
	}
	return result;
}

INT_PTR Candidates::AppendChildCandidate(INT_PTR candidate_index, const WCHAR*candidate, const WCHAR*description) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			auto &child = m_candidates.at(candidate_index).m_child;
			child.emplace_back(candidate, description);
			return child.size() - 1;
		}
		catch (std::exception) {
			//pass
		}		
	}
	return UNITY_NOT_FOUND_INDEX;
}

bool Candidates::SetActionDirectoryName(INT_PTR index, const WCHAR* directory_name) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_directory_name.assign(directory_name);
			return true;
		}
		catch (std::exception) {
			//pass
		}		
	}
	return false;
}

bool Candidates::SetActionFileName(INT_PTR index, const WCHAR* filename) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_file_name.assign(filename);
			return true;
		}
		catch (std::exception) {
			//pass
		}		
	}
	return false;
}

bool Candidates::SetActionColumn(INT_PTR index, INT_PTR column) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_column = column;
			return true;
		}
		catch (std::exception) {
			//pass
		}		
	}
	return false;
}

bool Candidates::SetActionLine(INT_PTR index, INT_PTR line) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_line = line;
			return true;
		}
		catch (std::exception) {
			//pass
		}	
	}
	return false;
}

bool Candidates::SetActionCommand(INT_PTR index, const WCHAR* command) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_command.assign(command);
			return true;
		}
		catch (std::exception) {
			//pass
		}
	}
	return false;
}

bool Candidates::SetActionProcessId(INT_PTR index, INT_PTR process_id) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_process_id=process_id;
			return true;
		}
		catch (std::exception) {
			//pass
		}
	}
	return false;
}

bool Candidates::SetActionWindowHandle(INT_PTR index, INT_PTR window_handle) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			m_candidates.at(index).m_action.m_window_handle = window_handle;
			return true;
		}
		catch (std::exception) {
			//pass
		}
	}
	return false;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, const WCHAR*data) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).SetUserData(key, data);
		}
		catch (std::exception) {
			//pass
		}		
	}
	return false;
}

bool Candidates::SetUserData(INT_PTR index, const WCHAR* key, INT_PTR data) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).SetUserData(key, data);
		}
		catch (std::exception) {
			//pass
		}		
	}
	return false;
}

const WCHAR*	Candidates::GetUserData(INT_PTR index, const WCHAR* key, const WCHAR*	default_data) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).GetUserData(key, default_data);
		}
		catch (std::exception) {
			//pass
		}		
	}
	return default_data;
}

INT_PTR			Candidates::GetUserData(INT_PTR index, const WCHAR* key, INT_PTR		default_data) {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).GetUserData(key, default_data);
		}
		catch (std::exception) {
			//pass
		}		
	}
	return default_data;
}

const WCHAR* Candidates::GetSourceName(INT_PTR index)const{
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_source_name.c_str();
		}
		catch (std::exception) {
			//pass
		}		
	}
	return nullptr;
}

const WCHAR* Candidates::GetText(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_text.c_str();
		}
		catch (std::exception) {
			//pass
		}		
	}
	return nullptr;
}

const WCHAR* Candidates::GetActionDirectoryName(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_directory_name.c_str();
		}
		catch (std::exception) {
			//pass
		}		
	}
	return nullptr;
}

const WCHAR* Candidates::GetActionFileName(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_file_name.c_str();
		}
		catch (std::exception) {
			//pass
		}		
	}
	return nullptr;
}

INT_PTR		Candidates::GetActionColumn(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_column;
		}
		catch (std::exception) {
			//pass
		}		
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR		Candidates::GetActionLine(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_line;
		}
		catch (std::exception) {
			//pass
		}
	}
	return UNITY_NOT_FOUND_INDEX;
}

const WCHAR* Candidates::GetActionCommand(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_command.c_str();
		}
		catch (std::exception) {
			//pass
		}
	}
	return _T("");
}

INT_PTR		Candidates::GetActionProcessId(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_process_id;
		}
		catch (std::exception) {
			//pass
		}
	}
	return UNITY_INVALID_PROCESS_ID;
}

INT_PTR		Candidates::GetActionWindowHandle(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_action.m_window_handle;
		}
		catch (std::exception) {
			//pass
		}
	}
	return UNITY_HIDEMARU_NULL_HANDLE;
}

const WCHAR* Candidates::GetDescription(INT_PTR index)const {
	ContainerType::scoped_lock locker(m_candidates);
	{
		try {
			return m_candidates.at(index).m_description.c_str();
		}
		catch (std::exception) {
			//pass
		}		
	}
	return nullptr;
}

bool Candidates::ClearWithSourceName(const WCHAR*sourcename) {
	ContainerType::scoped_lock locker(m_candidates);
	try {
		auto first = std::find_if(m_candidates.begin(), m_candidates.end(), [sourcename](auto&item) {return item.m_source_name.compare(sourcename) == 0; });
		if (first == m_candidates.end()) {
			return true;
		}
		auto last = std::find_if_not(first, m_candidates.end(), [sourcename](auto&item) {return item.m_source_name.compare(sourcename) == 0; });
		m_candidates.erase(first, last);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
const INT_PTR Candidates::GetSourceNameForCandidatesNum() const{
	return m_source_names_for_candidates.size();
}
const WCHAR* Candidates::GetSourceNameForCandidatesFromIndex(INT_PTR index)const {
	if ((index<0) || (m_source_names_for_candidates.size() <= index)) {
		return L"";
	}
	auto itr = m_source_names_for_candidates.begin();
	std::advance(itr, index);
    return itr->c_str();
}
const std::unordered_set<std::wstring>& Candidates::GetSourceNamesForCandidates() const {
	return m_source_names_for_candidates;
}
std::unordered_set<std::wstring>& Candidates::GetSourceNamesForCandidates() {
	return m_source_names_for_candidates;
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

