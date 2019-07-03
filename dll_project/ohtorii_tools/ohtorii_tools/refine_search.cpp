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
//	Filter
///////////////////////////////////////////////////////////////////////////////
class Filter {
public:
	Filter(HidemaruView& hidemaru_view) : m_hidemaru_view(hidemaru_view){

	};

	void Generate(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates) {
		m_hidemaru_view.Clear();
		m_hidemaru_view.m_collapsed.Clear();

		size_t			current_hidemaru_lineno = 1;//memo: 秀丸エディタの行番号は1スタート
		const size_t	size = candidates.size();
		bool			first_match = true;
		auto &			hidemaru_text = m_hidemaru_view.m_hidemaru_text;
		INT_PTR			collapsed_index = 0;

		for (size_t candidate_list_index = 0; candidate_list_index < size; ++candidate_list_index) {
			const auto& candidate = candidates.at(candidate_list_index);
			if (!MatchAll(candidate.m_text, tokens)) {
				continue;
			}

			if (first_match) {
				//開始行には改行を挿入しない
				first_match = false;
			}
			else {
				hidemaru_text.push_back(_T('\n'));
			}

			//
			//候補と詳細のテキストを追加する
			//
			hidemaru_text.insert(hidemaru_text.end(), candidate.m_text.begin(), candidate.m_text.end());
			if (!candidate.m_description.empty()) {
				hidemaru_text.push_back(_T('\t'));
				//hidemaru_text.push_back(_T('\t'));
				hidemaru_text.insert(hidemaru_text.end(), candidate.m_description.begin(), candidate.m_description.end());
			}
			if (candidate.m_selected) {
				m_hidemaru_view.m_hidemaru_maeked_lineno.push_back(current_hidemaru_lineno);
			}
			m_hidemaru_view.m_collapsed.OnChangeCollapsedIndex(current_hidemaru_lineno, collapsed_index);
			m_hidemaru_view.m_collapsed.OnChangeHidemaruLineNo(current_hidemaru_lineno, collapsed_index);
			m_hidemaru_view.m_hidemaru_lineno_to_candidate_list_index.push_back(candidate_list_index);


			//
			//子供のテキストと詳細を追加する
			//
			for (const auto&child : candidate.m_child) {
				hidemaru_text.push_back(_T('\n'));
				++current_hidemaru_lineno;

				hidemaru_text.push_back(_T('\t'));
				hidemaru_text.insert(hidemaru_text.end(), child.m_text.begin(), child.m_text.end());
				if (!child.m_description.empty()) {
					hidemaru_text.push_back(_T('\t'));
					//hidemaru_text.push_back(_T('\t'));
					hidemaru_text.insert(hidemaru_text.end(), child.m_description.begin(), child.m_description.end());
				}
				m_hidemaru_view.m_collapsed.OnChangeHidemaruLineNo(current_hidemaru_lineno, collapsed_index);
				m_hidemaru_view.m_hidemaru_lineno_to_candidate_list_index.push_back(candidate_list_index);
			}

			++collapsed_index;
			++current_hidemaru_lineno;
		}

		//テキストの終端を追加
		m_hidemaru_view.m_hidemaru_text.push_back(_T('\0'));
	};

private:
	HidemaruView&m_hidemaru_view;
};


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


bool RefineSearch::Do(const WCHAR* search_words) {
	try{
		std::vector<std::wstring> tokens;
		tokens.reserve(16);
		Tokenize(tokens, const_cast<WCHAR*>(search_words), _T(" \t\n"));

		auto& candidates= m_instance->QueryCandidates()->GetCandidates();
	
		//memo: std::vector<>のメモリ予約		
		m_hidemaru_view.Reserve(candidates.size());		
		
		Filter filter(m_hidemaru_view);
		filter.Generate(tokens, candidates);		
	}
	catch (std::exception) {
		return false;
	}
	return true;
}

WCHAR* RefineSearch::GetResult() {
	if(m_hidemaru_view.m_hidemaru_text.empty()){
		return gs_empty;
	}
	return &m_hidemaru_view.m_hidemaru_text.at(0);
}


INT_PTR RefineSearch::ChangeMarked(INT_PTR hidemaru_line_no, bool is_selected) {
	try {
		bool		has_change		= false;
		const auto	candidate_index = m_hidemaru_view.m_hidemaru_lineno_to_candidate_list_index.at(hidemaru_line_no - 1);//-1して0始まりにする
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
				auto find_it = std::find(m_hidemaru_view.m_hidemaru_maeked_lineno.begin(), m_hidemaru_view.m_hidemaru_maeked_lineno.end(), hidemaru_line_no);
				if (find_it != m_hidemaru_view.m_hidemaru_maeked_lineno.end()) {
					m_hidemaru_view.m_hidemaru_maeked_lineno.erase(find_it);
					has_change = true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_hidemaru_view.m_hidemaru_maeked_lineno.push_back(hidemaru_line_no);
				has_change = true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}

		if (has_change) {
			std::sort(m_hidemaru_view.m_hidemaru_maeked_lineno.begin(), m_hidemaru_view.m_hidemaru_maeked_lineno.end());
		}

		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR RefineSearch::GetMarkedCount() {
	return m_hidemaru_view.m_hidemaru_maeked_lineno.size();
}


INT_PTR RefineSearch::ConvertSelectedIndexToHidemaruLineno(INT_PTR marked_index) {
	try {
		return m_hidemaru_view.m_hidemaru_maeked_lineno.at(marked_index);
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no) {	
	DebugLog(_T("ConvertHidemaruLinenNoToCandidateIndex"));
	DebugLog(_T("  hidemaru_line_no=%d"),hidemaru_line_no);
	
	if (hidemaru_line_no <= 0) {
		DebugLog(_T("  return -1@1"));
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		auto value = m_hidemaru_view.m_hidemaru_lineno_to_candidate_list_index.at(hidemaru_line_no);
		DebugLog(_T("  return value=%d"),value);
		return value;
	}
	catch (std::exception) {
		//pass
	}
	DebugLog(_T("  return -1@2"));
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::ConvertMarkIndexToCandidatesIndex(INT_PTR marked_index) {
	INT_PTR hidemaru_line_no = ConvertSelectedIndexToHidemaruLineno(marked_index);
	if (hidemaru_line_no <= 0) {
		return UNITY_NOT_FOUND_INDEX;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		return m_hidemaru_view.m_hidemaru_lineno_to_candidate_list_index.at(hidemaru_line_no);
	}catch(std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}
/*
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
*/
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

	DebugLog(_T("GetSelectionCandidateIndex"));
	DebugLog(_T("  marked_count=%d"),marked_count);
	DebugLog(_T("  selected_index=%d"),selected_index);
	
	if (0 < marked_count) {
		return ConvertMarkIndexToCandidatesIndex(selected_index);
	}
	if (selected_index == 0) {
		return ConvertHidemaruLinenNoToCandidateIndex(m_hidemaru_line_no);
	}
	return UNITY_NOT_FOUND_INDEX;
}

INT_PTR RefineSearch::MoveHidemaruCursorLineNo(INT_PTR current_lineno, INT_PTR candidate_delta) {	
	return m_hidemaru_view.m_collapsed.CalcHidemaruCursorLineNo(current_lineno,candidate_delta);
}

