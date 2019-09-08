#include"stdafx.h"


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
	//m_reget_candidate_source_names.clear();
}

void StaticStatus::UpdateStatus(INT_PTR kind_index, INT_PTR action_index) {
	const auto &kind=Unity::Instance().lock()->QueryKinds();

	m_is_quit = kind.IsActionQuit(kind_index, action_index);

	auto is_start = kind.IsActionStart(kind_index, action_index);
	GetIsStart().SetEnable(is_start);
}

/*
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
*/
