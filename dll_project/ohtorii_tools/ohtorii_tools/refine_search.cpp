#include "stdafx.h"



///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };



///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static void Tokenizer(std::vector<std::wstring>&out, WCHAR* search_words) {
	//OutputDebugString(L"==== Tokenizer start ====");
	out.clear();
	if (wcslen(search_words) == 0) {
		//OutputDebugString(L"==== Tokenizer finish ====");
		return;
	}

	const WCHAR*	delimiters = L" \t\n";
	WCHAR*			next_token = NULL;
	WCHAR*			p = wcstok_s(search_words, delimiters, &next_token);
	while (p) {
		//	OutputDebugString(p);
		out.push_back(p);
		p = wcstok_s(NULL, delimiters, &next_token);
	}
	//	OutputDebugString(L"==== Tokenizer finish ====");
}


static bool MatchAll(const std::wstring &line, const std::vector<std::wstring>& tokens) {
	for (auto&word : tokens) {
		if (line.find(word) == std::wstring::npos) {
			return false;
		}
	}
	return true;
}

void RefineSearch::Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates) {
	//一致する行を返す
	size_t			hidemaru_lineno = 1;//memo: 秀丸エディタの行番号は1スタート
	const size_t	size			= candidates.size();
	for (size_t candidate_index = 0; candidate_index < size; ++candidate_index) {
		const auto& candidate = candidates.at(candidate_index);
		if (MatchAll(candidate.m_text, tokens)) {
			m_output.m_text.insert(m_output.m_text.end(), candidate.m_text.begin(), candidate.m_text.end());
			m_output.m_text.push_back(_T('\n'));

			m_output.m_hidemaru_lineno_to_candidate_index.push_back(candidate_index);
			if (candidate.m_selected) {
				m_output.m_hidemaru_selected_lineno.push_back(hidemaru_lineno);
			}

			++hidemaru_lineno;
		}
	}
	//OutputDebugString(L"==== Find finish ====");
}

bool RefineSearch::Do(const WCHAR* search_words) {
	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenizer(tokens, const_cast<WCHAR*>(search_words));

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


INT_PTR RefineSearch::ChangeSelected(INT_PTR hidemaru_line_no, bool is_selected) {
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
				auto find_it = std::find(m_output.m_hidemaru_selected_lineno.begin(), m_output.m_hidemaru_selected_lineno.end(), hidemaru_line_no);
				if (find_it != m_output.m_hidemaru_selected_lineno.end()) {
					m_output.m_hidemaru_selected_lineno.erase(find_it);
					has_change = true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_output.m_hidemaru_selected_lineno.push_back(hidemaru_line_no);
				has_change = true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}

		if (has_change) {
			std::sort(m_output.m_hidemaru_selected_lineno.begin(), m_output.m_hidemaru_selected_lineno.end());
		}

		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR RefineSearch::GetSelectionCount() {
	return m_output.m_hidemaru_selected_lineno.size();
}


INT_PTR RefineSearch::GetSelectedLineno(INT_PTR index) {
	try {
		return m_output.m_hidemaru_selected_lineno.at(index);
	}
	catch (std::exception) {
		//pass
	}
	return -1;
}


WCHAR* 	RefineSearch::GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no) {
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
}


WCHAR* 	RefineSearch::GetSelectedFilename(INT_PTR index) {
	Candidate*candidate = GetSelectedCandidate(index);
	if (candidate == nullptr) {
		return gs_empty;
	}
	return &candidate->m_text.at(0);
	//INT_PTR hidemaru_line_no = GetSelectedLineno(index);
	//return GetSelectedFilenameFromHidemaruLineNo(hidemaru_line_no);
}

Candidate* RefineSearch::GetSelectedCandidate(INT_PTR index) {
	INT_PTR hidemaru_line_no = GetSelectedLineno(index);
	if (hidemaru_line_no <= 0) {
		return nullptr;
	}

	--hidemaru_line_no;//0始まりにする
	try {
		const auto filelist_lineno = m_output.m_hidemaru_lineno_to_candidate_index.at(hidemaru_line_no);
		return &(m_instance->QueryCandidates()->GetCandidates().at(filelist_lineno));
	}
	catch (std::exception) {
		//pass
	}
	return nullptr;
}
