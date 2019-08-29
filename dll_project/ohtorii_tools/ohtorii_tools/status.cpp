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
