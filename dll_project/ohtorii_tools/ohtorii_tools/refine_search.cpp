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
	for (const std::wstring&word : tokens) {
		if (line.find(word) == std::wstring::npos) {
			return false;
		}
	}
	return true;
}

void RefineSearch::Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates) {
	//一致する行を返す
	size_t			select_lineno = 0;
	const size_t	input_size = candidates.size();
	for (size_t input_lineno = 0; input_lineno < input_size; ++input_lineno) {
		const auto& line = candidates.at(input_lineno);
		if (MatchAll(line.m_text, tokens)) {
			m_output.m_text.insert(m_output.m_text.end(), line.m_text.begin(), line.m_text.end());
			m_output.m_text.push_back(_T('\n'));
			if (line.m_selected) {
				m_output.m_selected_lineno.push_back(select_lineno);
			}
			m_output.m_hidemaru_to_filelist_lineno.push_back(input_lineno);
			++select_lineno;
		}
	}
	
	//OutputDebugString(L"==== Find finish ====");
}

bool RefineSearch::Do(const WCHAR* search_words) {
	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenizer(tokens, const_cast<WCHAR*>(search_words));

	auto candidates=Unity::Instance()->QueryCandidates()->GetCandidates();
	
	//memo: std::vector<>のメモリ予約		
	m_output.Reserve(candidates.size());
	m_output.Clear();

	Filter(tokens, candidates);
	//テキストの終端を追加
	m_output.m_text.push_back(L'\0');
	return true;
}

WCHAR* RefineSearch::GetResult() {
	return &m_output.m_text.at(0);
}


INT_PTR RefineSearch::ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected) {
	try {
		bool has_change = false;
		const auto filelist_lineno = m_output.m_hidemaru_to_filelist_lineno.at(hidemaru_line_no - 1);//-1して0始まりにする
		auto candidates = Unity::Instance()->QueryCandidates()->GetCandidates();
		const bool old = candidates.at(filelist_lineno).m_selected;
		candidates.at(filelist_lineno).m_selected = is_selected;

		if (old) {
			if (is_selected) {
				//select -> select 
				//pass
			}
			else {
				//select -> unselect
				auto find_it = std::find(m_output.m_selected_lineno.begin(), m_output.m_selected_lineno.end(), hidemaru_line_no);
				if (find_it != m_output.m_selected_lineno.end()) {
					m_output.m_selected_lineno.erase(find_it);
					has_change = true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_output.m_selected_lineno.push_back(hidemaru_line_no);
				has_change = true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}

		if (has_change) {
			std::sort(m_output.m_selected_lineno.begin(), m_output.m_selected_lineno.end());
		}

		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR RefineSearch::GetSelectionCount() {
	return m_output.m_selected_lineno.size();
}


INT_PTR RefineSearch::GetSelectedLineno(INT_PTR index) {
	try {
		return m_output.m_selected_lineno.at(index);
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
		const auto filelist_lineno = m_output.m_hidemaru_to_filelist_lineno.at(hidemaru_line_no);
		return &(Unity::Instance()->QueryCandidates()->GetCandidates().at(filelist_lineno).m_text.at(0));
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}


WCHAR* 	RefineSearch::GetSelectedFilename(INT_PTR index) {
	INT_PTR hidemaru_line_no = GetSelectedLineno(index);
	return GetSelectedFilenameFromHidemaruLineNo(hidemaru_line_no);
}

