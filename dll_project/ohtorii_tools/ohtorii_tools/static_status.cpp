#include"stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//	Static functions.
/////////////////////////////////////////////////////////////////////////////

/* 選択した候補からソース名を取得する
*/
static void GatherSourceNames(std::vector<std::wstring>&out, const RefineSearch&search, const Candidates&candidates) {
	std::unordered_set<std::wstring>	source_names;
	{
		const INT_PTR selections = search.GetSelectionCandidateCount();
		for (INT_PTR selection_index = 0; selection_index < selections; ++selection_index) {
			auto candidate_index = search.GetSelectionCandidateIndex(selection_index);
			auto source_name = candidates.GetSourceName(candidate_index);
			source_names.insert(source_name);
		}
	}
	out.resize(source_names.size());
	std::copy(source_names.cbegin(), source_names.cend(), out.begin());
}

/////////////////////////////////////////////////////////////////////////////
//	Status::IsStart
/////////////////////////////////////////////////////////////////////////////
StaticStatus::IsStart::IsStart() {
	m_is_start = false;
}

bool StaticStatus::IsStart::Set(const WCHAR*source_name, const WCHAR*arg) {
	try {
		m_source_name.assign(source_name);
		m_arg.assign(arg);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
};

bool StaticStatus::IsStart::SetEnable(bool is_start) {
	m_is_start = is_start;
	return true;
}

bool StaticStatus::IsStart::Clear() {
	try {
		m_is_start = false;
		m_source_name.clear();
		m_arg.clear();
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool StaticStatus::IsStart::Enable()const {
	if ((!m_is_start) || m_source_name.empty()) {
		return false;
	}
	return true;
};


/////////////////////////////////////////////////////////////////////////////
//	Status
/////////////////////////////////////////////////////////////////////////////
StaticStatus::StaticStatus() {
	m_is_quit = false;
	m_hidemaruhandle_to_focus_at_end_of_process = UNITY_HIDEMARU_NULL_HANDLE;
	m_target_hidemaruhandle = UNITY_HIDEMARU_NULL_HANDLE;

}

StaticStatus::IsStart&	StaticStatus::GetIsStart() {
	return m_is_start;
};

const StaticStatus::IsStart&	StaticStatus::GetIsStart()const {
	return m_is_start;
};

void StaticStatus::Initialize(INT_PTR target_hidemaru, const WCHAR* working_directory, const WCHAR*root_macro_directory) {
	m_hidemaruhandle_to_focus_at_end_of_process = UNITY_HIDEMARU_NULL_HANDLE;
	m_target_hidemaruhandle = target_hidemaru;
	m_current_working_directory.assign(working_directory);
	m_root_macro_directory.assign(root_macro_directory);
	m_is_quit = true;
	m_reget_candidate_source_names.clear();
}

bool StaticStatus::Reset(const WCHAR*kind_name, const WCHAR*action_name) {
	auto unity = Unity::Instance().lock();
	auto&kind=unity->QueryKinds();

	auto kind_index=kind.FindKindIndex(kind_name);
	if (kind_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	
	auto action_index=kind.FindActionIndex(kind_index, action_name);
	if (action_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}

	m_is_quit= kind.IsActionQuit(kind_index,action_index);

	m_reget_candidate_source_names.clear();
	if (kind.IsRegetCandidates(kind_index, action_index)) {
		GatherSourceNames(
			m_reget_candidate_source_names, 
			unity->QueryRefineSearch(), 
			unity->QueryCandidates());
	}
	
	auto is_start = kind.IsActionStart(kind_index, action_index);
	GetIsStart().SetEnable(is_start);

	return true;
}

INT_PTR StaticStatus::GetNumberOfSourceNamesForReacquisitionCandidates()const {
	return m_reget_candidate_source_names.size();
}

const WCHAR* StaticStatus::GetSourceNameForReacquisitionCandidates(INT_PTR index)const {
	try {
		return m_reget_candidate_source_names.at(index).c_str();
	}
	catch (std::exception) {
		//pass
	}
	return _T("");
}