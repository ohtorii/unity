#pragma once

/*コンテキスト毎の状態
*/
class ContextStatus {
public:
	template<class Archive> void serialize(Archive & archive) {
		archive(m_reget_candidate_source_names);
	};

	ContextStatus();

	void			UpdateStatus(INT_PTR kind_index, INT_PTR action_index, INT_PTR context_index);
	INT_PTR			GetNumberOfSourceNamesForReacquisitionCandidates()const;
	const WCHAR*	GetSourceNameForReacquisitionCandidates(INT_PTR index)const;
private:
	std::vector<std::wstring>	m_reget_candidate_source_names;
};