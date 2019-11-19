#include"stdafx.h"

bool			UserData::SetUserData(const WCHAR* key, const WCHAR*data) {
	return m_string.emplace(key, data).second;
}

const WCHAR*	UserData::GetUserData(const WCHAR* key, const WCHAR*	default_data)const {
	const auto it = m_string.find(key);
	if (it == m_string.end()) {
		return default_data;
	}
	return it->second.c_str();
}
