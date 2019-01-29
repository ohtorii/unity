// ohtorii_tools.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//
#include "stdafx.h"

//
//memo: 検索対処の文字列は一行ずつ管理します。
//

//秀丸エディタから受け取った情報
struct Input{
	Input() {
		m_selected = false;
	};
	Input(const WCHAR*wstr) : m_text(wstr){
		m_selected = false;
	};

	//ファイルリストから読み込んだファイル名
	std::wstring			m_text;
	//このテキストが選択されているかどうか
	bool					m_selected;
};
static std::vector<Input>	gs_input;


//秀丸エディタへ返す情報
struct Output{
	void Clear() {
		m_text.clear();
		m_hidemaru_to_filelist_lineno.clear();
		m_selected_lineno.clear();
	};
	void Reserve(size_t size) {
		m_text.reserve(size);
		m_hidemaru_to_filelist_lineno.reserve(size);
		m_selected_lineno.reserve(32);
	};

	//秀丸エディタへ返す文字列
	std::vector<std::wstring::value_type>	m_text;	
	
	/*「秀丸エディタの行番号」から「ファイルリストの行番号」を取得するテーブル
	
	(使用例)
	ファイルリストの行番号 = m_hidemaru_to_filelist_lineno[秀丸エディタの行番号];
	*/
	std::vector<__int64>					m_hidemaru_to_filelist_lineno;

	//秀丸エディタで選択する行番号
	std::vector<__int64>					m_selected_lineno;
};
static Output	gs_output;

static WCHAR	gs_empty[] = { 0 };

static bool LoadTextFile(const WCHAR*filename) {
	//OutputDebugString(L"==== Loadtext start ====");
	//OutputDebugString(filename);
	gs_input.clear();

	FILE *fp = 0;
	const errno_t error = _wfopen_s(&fp, filename, L"rb");
	if (error != 0) {
		//OutputDebugString(L"@1");
		return false;
	}	

	bool result = false;
	WCHAR line[64 * 1024];
	try {
		if (gs_input.capacity() == 0) {
			gs_input.reserve(1024);
		}
		while (true) {
			if (fgetws(line, _countof(line), fp) == 0) {
				break;
			}
			//OutputDebugString(line);
			gs_input.emplace_back(line);
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

/*行を「選択・選択解除」する
hidemaru_line_no	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
　					ファイルの先頭が１です。 
*/
extern "C" INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected) {
	--hidemaru_line_no;//0始まりにする
	try {
		const auto filelist_lineno = gs_output.m_hidemaru_to_filelist_lineno.at(hidemaru_line_no);
		gs_input.at(filelist_lineno).m_selected = is_selected;
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

/*選択行の個数を取得する
*/
extern "C" INT_PTR GetSelectionCount() {
	return gs_output.m_selected_lineno.size();
}

/*選択行を取得する
return	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
　		ファイルの先頭が１です。 
*/
extern "C" INT_PTR GetSelectedLineno(INT_PTR index) {
	try {
		return gs_output.m_selected_lineno.at(index) + 1;
	}catch(std::exception) {
		//pass
	}
	return -1;
}

/*ファイルリストのファイル名を設定する
return	bool	true	成功
				false	失敗
*/
extern "C" UINT_PTR SetFileListName(WCHAR* filename) {
	return LoadTextFile(filename);
}

/*
search_words	スペース区切りの検索文字列
				(Ex.) "c: programs .txt"
返値		マッチした文字列
*/
extern "C" WCHAR* Filter(WCHAR* search_words) {
	//OutputDebugString(L"====Enter====");
	//OutputDebugString(search_words);
	//LoadTextFile();
	
	gs_output.Reserve(gs_input.size());
	gs_output.Clear();

	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenizer(tokens, search_words);
	//std::sort(token.begin(), token.end());

	//OutputDebugString(L"==== Find start ====");
	/*if (tokens.size()==0) {
		//全ての行を返す
		const size_t size = gs_input.size();
		for (size_t input_lineno = 0; input_lineno < size; ++input_lineno) {
			const auto& line = gs_input.at(input_lineno);
			gs_output.m_text.insert(gs_output.m_text.end(), line.m_text.begin(), line.m_text.end());
			if (line.m_selected) {
				gs_output.m_selected_lineno.push_back(input_lineno);
			}
			gs_output.m_hidemaru_to_filelist_lineno.push_back(input_lineno);
		}
	}else*/
	{
		//一致する行を返す
		size_t			select_lineno	= 0;
		const size_t	input_size		= gs_input.size();
		for (size_t input_lineno = 0; input_lineno < input_size; ++input_lineno) {
			const auto& line = gs_input.at(input_lineno);
			if (MatchAll(line.m_text, tokens)) {
				gs_output.m_text.insert(gs_output.m_text.end(), line.m_text.begin(), line.m_text.end());
				if (line.m_selected) {
					gs_output.m_selected_lineno.push_back(select_lineno);
				}
				gs_output.m_hidemaru_to_filelist_lineno.push_back(input_lineno);
				++select_lineno;				
			}
		}
	}
	//OutputDebugString(L"==== Find finish ====");
	if (gs_output.m_text.size() == 0) {
		return gs_empty;
	}else {
		gs_output.m_text.push_back(L'\0');
	}

//	OutputDebugString(L"==== Result start ====");
	//OutputDebugString(&gs_result_text[0]);	
	//OutputDebugString(L"==== Result finish ====");
	return &gs_output.m_text[0];
}
