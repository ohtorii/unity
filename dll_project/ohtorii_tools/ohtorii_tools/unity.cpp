#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };
Unity*	Unity::m_instance=0;


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

	const WCHAR*	delimiters	= L" \t\n";
	WCHAR*			next_token	= NULL;
	WCHAR*			p			= wcstok_s(search_words, delimiters,&next_token);
	while (p) {
	//	OutputDebugString(p);
		out.push_back(p);
		p = wcstok_s(NULL, delimiters, &next_token);
	}
//	OutputDebugString(L"==== Tokenizer finish ====");
}

static bool MatchAll(const std::wstring &line,  const std::vector<std::wstring>& tokens) {
	for (const std::wstring&word:tokens) {
		if (line.find(word) == std::wstring::npos) {
			return false;
		}
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
Unity* Unity::Instance(){
	if(Unity::m_instance==nullptr){
		Unity::m_instance=new Unity;
	}
	return Unity::m_instance;
}

Sources* Unity::QuerySources(){
	return &m_sources;
}

File*	Unity::QueryFile(){
	return &m_file;
}

Candidates*	Unity::QueryCandidates() {
	return &m_candidates;
}

Unity::Unity(){
	
}

Unity::~Unity(){
	
}

INT_PTR Unity::ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected){
	try {
		bool has_change=false;
		const auto filelist_lineno = m_output.m_hidemaru_to_filelist_lineno.at(hidemaru_line_no - 1);//-1して0始まりにする
		const bool old = m_input.at(filelist_lineno).m_selected;
		m_input.at(filelist_lineno).m_selected = is_selected;

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
					has_change=true;
				}
			}
		}
		else {
			if (is_selected) {
				//unselect -> select
				m_output.m_selected_lineno.push_back(hidemaru_line_no);
				has_change=true;
			}
			else {
				//unselect -> unselect
				//pass
			}
		}
		
		if(has_change){
			std::sort(m_output.m_selected_lineno.begin(), m_output.m_selected_lineno.end());
		}
		
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}


INT_PTR Unity::GetSelectionCount(){
	return m_output.m_selected_lineno.size();
}


INT_PTR Unity::GetSelectedLineno(INT_PTR index){
	try {
		return m_output.m_selected_lineno.at(index);
	}catch(std::exception) {
		//pass
	}
	return -1;
}


WCHAR* 	Unity::GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no){
	if (hidemaru_line_no <= 0) {
		return gs_empty;
	}
	--hidemaru_line_no;//0始まりにする
	try {
		const auto filelist_lineno = m_output.m_hidemaru_to_filelist_lineno.at(hidemaru_line_no);
		return &(m_input.at(filelist_lineno).m_text.at(0));
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}


WCHAR* 	Unity::GetSelectedFilename(INT_PTR index){
	INT_PTR hidemaru_line_no = GetSelectedLineno(index);
	return GetSelectedFilenameFromHidemaruLineNo(hidemaru_line_no);
}


INT_PTR Unity::SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description){
	return LoadTextFile(source_filename);
}


WCHAR* 	Unity::Filter(WCHAR* search_words){
	//OutputDebugString(L"====Enter====");
	//OutputDebugString(search_words);
	//LoadTextFile();

	m_output.Reserve(m_input.size());
	m_output.Clear();

	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenizer(tokens, search_words);
	
	{
		//一致する行を返す
		size_t			select_lineno	= 0;
		const size_t	input_size		= m_input.size();
		for (size_t input_lineno = 0; input_lineno < input_size; ++input_lineno) {
			const auto& line = m_input.at(input_lineno);
			if (MatchAll(line.m_text, tokens)) {
				m_output.m_text.insert(m_output.m_text.end(), line.m_text.begin(), line.m_text.end());
				if (line.m_selected) {
					m_output.m_selected_lineno.push_back(select_lineno);
				}
				m_output.m_hidemaru_to_filelist_lineno.push_back(input_lineno);
				++select_lineno;
			}
		}
	}
	//OutputDebugString(L"==== Find finish ====");
	if (m_output.m_text.size() == 0) {
		return gs_empty;
	}else {
		m_output.m_text.push_back(L'\0');
	}

//	OutputDebugString(L"==== Result start ====");
	//OutputDebugString(&gs_result_text[0]);
	//OutputDebugString(L"==== Result finish ====");
	return &m_output.m_text[0];
}


bool Unity::LoadTextFile(const WCHAR*filename) {
	//OutputDebugString(L"==== Loadtext start ====");
	//OutputDebugString(filename);
	m_input.clear();

	FILE *fp = 0;
	const errno_t error = _wfopen_s(&fp, filename, L"rb");
	if (error != 0) {
		//OutputDebugString(L"@1");
		return false;
	}
	if (fp == 0) {
		return false;
	}

	bool result = false;
	std::vector<WCHAR> line;
	line.resize(64 * 1024);
	try {
		if (m_input.capacity() == 0) {
			m_input.reserve(1024);
		}
		while (true) {
			if (fgetws(&line[0], line.size(), fp) == 0) {
				break;
			}
			//OutputDebugString(line);
			m_input.emplace_back(&line[0]);
		}
		//OutputDebugString(L"@2");
		result = true;
	}
	catch (std::exception) {
		//pass
		//OutputDebugString(L"@3");
		result = false;
	}

	fclose(fp);
	//std::sort(gs_text.begin(), gs_text.end());
	//OutputDebugString(L"==== Loadtext finish ====");
	//OutputDebugString(L"@4");
	return result;
}



