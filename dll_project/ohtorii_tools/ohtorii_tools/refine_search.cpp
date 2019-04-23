#include "stdafx.h"



///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };



///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static bool MatchAll(const std::wstring &line, const std::vector<std::wstring>& tokens) {
	for (auto&word : tokens) {
		if (line.find(word) == std::wstring::npos) {
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//	RefineSearch
///////////////////////////////////////////////////////////////////////////////
RefineSearch::RefineSearch(Unity*instance) : 
	m_instance(instance), 
	m_hidemaru_line_no(0) 
{
}

void RefineSearch::SetHidemaruLineno(INT_PTR hidemaru_line_no) {
	m_hidemaru_line_no = hidemaru_line_no;
}

void RefineSearch::Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates) {
	//一致する行を返す
	size_t			hidemaru_lineno = 1;//memo: 秀丸エディタの行番号は1スタート
	const size_t	size			= candidates.size();
	for (size_t candidate_index = 0; candidate_index < size; ++candidate_index) {
		const auto& candidate = candidates.at(candidate_index);
		if (MatchAll(candidate.m_text, tokens)) {
			m_output.m_text.insert(m_output.m_text.end(), candidate.m_text.begin(), candidate.m_text.end());
			
			if (! candidate.m_description.empty()) {
				m_output.m_text.push_back(_T('\t'));
				m_output.m_text.insert(m_output.m_text.end(), candidate.m_description.begin(), candidate.m_description.end());
			}

			m_output.m_text.push_back(_T('\n'));

			m_output.m_hidemaru_lineno_to_candidate_index.push_back(candidate_index);
			if (candidate.m_selected) {
				m_output.m_hidemaru_maeked_lineno.push_back(hidemaru_lineno);
			}

			++hidemaru_lineno;
		}
	}
	//OutputDebugString(L"==== Find finish ====");
}

bool RefineSearch::Do(const WCHAR* search_words) {
	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenize(tokens, const_cast<WCHAR*>(search_words), _T(" \t\n"));

	auto& candidates= m_instance->QueryCandidates()->GetCandidates();
	
	//memo: std::vector<>のメモリ予約		
	m_output.Reserve(candidates.size());
	m_output.Clear();

	Filter(tokens, candidates);
	//テキストの終端を追加
	m_output.m_text.push_back(_T('\0'));
	return true;
}

WCHAR* RefineSearch::GetResult() {
	return &m_output.m_text.at(0);
}


INT_PTR RefineSearch::ChangeMarked(INT_PTR hidemaru_line_no, bool is_selected) {
	try {
		bool		has_change		= false;
		const auto	candidate_index = m_output.m_hidemaru_lineno_to_candidate_index.at(hidemaru_line_no - 1);//-1して0始まりにする
		auto&		candidates		= m_instance->QueryCandidates()->GetCandidates();
		const bool	now				= candidates.at(candidate_index).m_selected;
		
		candidates.at(candidate_index).m_selected = is_selected;

		if (now) {
			if (is_selected) {
				//select -> select 
				//pass
			}
			else {
				//select -> unselect
				auto find_it = std::find(m_output.m_hidemaru_maeked_lineno.begin(), m_output.m_hidemaru_maeked_lineno.end(), hidemaru_line_no);
				if (find_it != m_output.m_hidemaru_maeked_lineno.end()) {
					m_output.m_hidemaru_maeked_lineno.erase(find_it);
					has_change = true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_output.m_hidemaru_maeked_lineno.push_back(hidemaru_line_no);
				has_change = true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}

		if (has_change) {
			std::sort(m_output.m_hidemaru_maeked_lineno.begin(), m_output.m_hidemaru_maeked_lineno.end());
		}

		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR RefineSearch::GetMarkedCount() {
	return m_output.m_hidemaru_maeked_lineno.size();
}


INT_PTR RefineSearch::ConvertSelectedIndexToHidemaruLineno(INT_PTR marked_index) {
	try {
		return m_output.m_hidemaru_maeked_lineno.at(marked_index);
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}


/*WCHAR* 	RefineSearch::GetMarkedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no) {
	if (hidemaru_line_no <= 0) {
		return gs_empty;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		const auto filelist_lineno = m_output.m_hidemaru_lineno_to_candidate_index.at(hidemaru_line_no);
		return &(m_instance->QueryCandidates()->GetCandidates().at(filelist_lineno).m_text.at(0));
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}*/

INT_PTR RefineSearch::ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no) {	
	if (hidemaru_line_no <= 0) {
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		return m_output.m_hidemaru_lineno_to_candidate_index.at(hidemaru_line_no);
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::ConvertMarkIndexToCandidatesIndex(INT_PTR marked_index) {
	INT_PTR hidemaru_line_no = ConvertSelectedIndexToHidemaruLineno(marked_index);
	if (hidemaru_line_no <= 0) {
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		return m_output.m_hidemaru_lineno_to_candidate_index.at(hidemaru_line_no);
	}catch(std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

/*WCHAR* 	RefineSearch::GetSelectedFilename(INT_PTR marked_index) {
	Candidate*candidate = GetMarkedCandidate(marked_index);
	if (candidate == nullptr) {
		return gs_empty;
	}
	return &candidate->m_text.at(0);
	//INT_PTR hidemaru_line_no = ConvertSelectedIndexToHidemaruLineno(index);
	//return GetMarkedFilenameFromHidemaruLineNo(hidemaru_line_no);
}*/

Candidate* RefineSearch::GetMarkedCandidate(INT_PTR marked_index) {	
	const auto candidate_index = ConvertMarkIndexToCandidatesIndex(marked_index);
	if (candidate_index == UNITY_NOT_FOUND_INDEX) {
		return nullptr;
	}
	try {
		return &(m_instance->QueryCandidates()->GetCandidates().at(candidate_index));
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}

INT_PTR	RefineSearch::GetFirstSelectionCandidateIndex() {
	if (GetMarkedCount()) {
		const INT_PTR first_item = 0;
		return ConvertMarkIndexToCandidatesIndex(first_item);
	}	
	return ConvertHidemaruLinenNoToCandidateIndex(m_hidemaru_line_no);
}

INT_PTR	RefineSearch::GetSelectionCandidateCount() {
	auto marked_count = GetMarkedCount();
	if (0<marked_count) {
		return marked_count;
	}
	if (m_hidemaru_line_no <= 0) {
		return 0;
	}
	return 1;
}

INT_PTR	RefineSearch::GetSelectionCandidateIndex(INT_PTR selected_index) {
	auto marked_count = GetMarkedCount();
	if (0 < marked_count) {
		return ConvertMarkIndexToCandidatesIndex(selected_index);
	}
	if (selected_index == 0) {
		return ConvertHidemaruLinenNoToCandidateIndex(m_hidemaru_line_no);
	}
	return UNITY_NOT_FOUND_INDEX;
}
