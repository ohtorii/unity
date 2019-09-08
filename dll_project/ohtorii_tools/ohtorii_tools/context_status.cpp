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
//	ContextStatus
/////////////////////////////////////////////////////////////////////////////
ContextStatus::ContextStatus() {
	
}

void ContextStatus::UpdateStatus(INT_PTR kind_index, INT_PTR action_index) {
	m_reget_candidate_source_names.clear();
	auto unity = Unity::Instance().lock();
	const auto &kind=unity->QueryKinds();
	if (kind.IsRegetCandidates(kind_index, action_index)) {
		GatherSourceNames(
			m_reget_candidate_source_names,
			unity->QueryRefineSearch(),
			unity->QueryCandidates());
	}
}

INT_PTR					ContextStatus::GetNumberOfSourceNamesForReacquisitionCandidates()const {
	return m_reget_candidate_source_names.size();
}

const WCHAR*		ContextStatus::GetSourceNameForReacquisitionCandidates(INT_PTR index)const {
	try {
		return m_reget_candidate_source_names.at(index).c_str();
	}
	catch (std::exception) {
		//pass
	}
	return _T("");
}

