#include"stdafx.h"



InterfaceSugar InterfaceSugar::m_instance;


InterfaceSugar& InterfaceSugar::Instance() {
	return m_instance;
}

InterfaceSugar::InterfaceSugar() {
	m_current_candidate_index = UNITY_NOT_FOUND_INDEX;
}

bool InterfaceSugar::SetCurrenSourceName(const WCHAR*source_name) {
	try {
		m_current_source_name.assign(source_name);
		return true;
	}catch (std::exception) {
		//pass
	}
	return false;
}

bool InterfaceSugar::ClearCurrentSourceName() {
	try {
		m_current_source_name.clear();
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

bool InterfaceSugar::AppendCandidateHeader(const WCHAR*header, const WCHAR*description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendCandidateHeader(m_current_source_name.c_str(), header, description);
	m_current_candidate_index = index;
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}

bool InterfaceSugar::AppendCandidate(const WCHAR*candidate, const WCHAR*description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendCandidate(m_current_source_name.c_str(), candidate, description);	
	m_current_candidate_index = index;
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}

bool InterfaceSugar::AppendCandidateAsASyncFile(const WCHAR*filename) {
	auto index = Unity::Instance().lock()->QueryASyncFiles().AppendCandidate(m_current_source_name.c_str(), filename);
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}
bool InterfaceSugar::AppendCandidateAsString(const WCHAR*string) {
	const auto current_source_name	= m_current_source_name.c_str();	

	std::vector<std::wstring> tokens;
	Tokenize(tokens, string, _T("\n"));

	auto& candidates_obj= Unity::Instance().lock()->QueryCandidates();
	auto& candidates	= candidates_obj.GetCandidates();

	{
		Candidates::ContainerType::scoped_lock	locker(candidates);

		for (auto &token : tokens) {
			token = TrimString(token);
			if (token.size() == 0) {
				continue;
			}

			const auto position_1st = token.find(_T("\t"));
			if (position_1st != std::wstring::npos) {
				const auto position_2nd = token.find(_T("\t"), position_1st + 1);
				if (position_2nd != std::wstring::npos) {

					/*\tが二つあるので以下フォーマット
					候補\t@action種類\taction種類に応じた書式
					*/

					//候補を取り出す
					std::wstring text(	token.begin(), 
										token.begin() + position_1st);
					candidates.emplace_back(current_source_name, text.c_str());

					//アクションを取り出す
					std::wstring action_name(	token.begin() + position_1st + 1,
												token.begin() + position_2nd);

					std::wstring action_text(	token.begin() + position_2nd + 1, 
												token.end());

					auto&	candidate	= candidates.back();
					auto&	action		= candidate.m_action;

					//アクションの引数を取り出す
					if (action_name == _T("@action_directory")) {
						action.m_directory_name.append(action_text);
					}
					else if (action_name == _T("@action_filename")) {
						action.m_file_name.append(action_text);
					}
					else if (action_name == _T("@description")) {
						candidate.m_description.assign(action_text);
					}
					else {
						//return false;
						//pass
					}

					continue;
				}
			}

			{
				auto index = candidates_obj.AppendCandidate(current_source_name, token.c_str());
				if (index == UNITY_NOT_FOUND_INDEX) {
					return false;
				}
			}

		}
	}

	return true;
}

bool InterfaceSugar::SetCandidateActionDirectoryName(const WCHAR*directory_name) {
	return Unity::Instance().lock()->QueryCandidates().SetActionDirectoryName(m_current_candidate_index, directory_name);
}

bool InterfaceSugar::SetCandidateActionFileName(const WCHAR*filename) {
	return Unity::Instance().lock()->QueryCandidates().SetActionFileName(m_current_candidate_index, filename);
}

bool InterfaceSugar::SetCandidateActionColumn(INT_PTR column) {
	return Unity::Instance().lock()->QueryCandidates().SetActionColumn(m_current_candidate_index, column);
}

bool InterfaceSugar::SetCandidateActionLine(INT_PTR line) {
	return Unity::Instance().lock()->QueryCandidates().SetActionLine(m_current_candidate_index, line);
}

bool InterfaceSugar::SetCandidateActionCommand(const WCHAR*command) {
	return Unity::Instance().lock()->QueryCandidates().SetActionCommand(m_current_candidate_index, command);
}

bool InterfaceSugar::SetCandidateActionProcessId(INT_PTR process_id) {
	return Unity::Instance().lock()->QueryCandidates().SetActionProcessId(m_current_candidate_index, process_id);
}

bool InterfaceSugar::SetCandidateActionWindowHandle(INT_PTR window_handle) {
	return Unity::Instance().lock()->QueryCandidates().SetActionWindowHandle(m_current_candidate_index, window_handle);
}

bool InterfaceSugar::SetCandidateUserDataString(const WCHAR* key, const WCHAR* data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(m_current_candidate_index, key, data);
}

bool InterfaceSugar::SetCandidateUserDataNumeric(const WCHAR* key, INT_PTR data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(m_current_candidate_index, key, data);
}

bool InterfaceSugar::AppendChildCandidate(const WCHAR* candidate, const WCHAR* description) {
	auto index = Unity::Instance().lock()->QueryCandidates().AppendChildCandidate(m_current_candidate_index,candidate,description);
	if (index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	return true;
}
