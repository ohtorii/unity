#include"stdafx.h"





/////////////////////////////////////////////////////////////////////////////
//	ContextStatus
/////////////////////////////////////////////////////////////////////////////
ContextStatus::ContextStatus() {
	
}

void ContextStatus::UpdateStatus(INT_PTR kind_index, INT_PTR action_index, INT_PTR context_index) {
	
}

/*
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
*/
