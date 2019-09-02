#include"stdafx.h"



Status::Status() {
	m_is_quit=false;
	m_hidemaruhandle_to_focus_at_end_of_process= UNITY_HIDEMARU_NULL_HANDLE;
	m_target_hidemaruhandle= UNITY_HIDEMARU_NULL_HANDLE;

}

bool Status::IsStart::Set(const WCHAR*source_name, const WCHAR*arg) {
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

bool Status::IsStart::SetEnable(bool is_start) {
	m_is_start = is_start;
	return true;
}

bool Status::IsStart::Clear() {
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

bool Status::IsStart::Enable()const {
	if ((!m_is_start) || m_source_name.empty()) {
		return false;
	}
	return true;
};

Status::IsStart&	Status::GetIsStart() {
	return m_is_start;
};

const Status::IsStart&	Status::GetIsStart()const {
	return m_is_start;
};

void Status::Initialize(INT_PTR target_hidemaru, const WCHAR* working_directory, const WCHAR*root_macro_directory) {
	m_hidemaruhandle_to_focus_at_end_of_process = UNITY_HIDEMARU_NULL_HANDLE;
	m_target_hidemaruhandle = target_hidemaru;
	m_current_working_directory.assign(working_directory);
	m_root_macro_directory.assign(root_macro_directory);
	m_is_quit = true;
}

bool Status::Reset(const WCHAR*kind_name, const WCHAR*action_name) {
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
	auto is_start = kind.IsActionStart(kind_index, action_index);
	GetIsStart().SetEnable(is_start);

	return true;
}
