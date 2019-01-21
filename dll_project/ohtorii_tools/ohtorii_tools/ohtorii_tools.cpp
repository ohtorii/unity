// ohtorii_tools.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//
#include "stdafx.h"

//memo: 検索対処の文字列を一行ずつ管理する
static std::vector<std::wstring>				gs_text;
static std::vector<std::wstring::value_type>	gs_result_text;
static WCHAR									gs_empty[] = { 0 };

static void LoadTextFile() {
	if (gs_text.size() != 0) {
		return;
	}
	FILE *fp = 0;
	const errno_t error = fopen_s(&fp, "C:\\Users\\ikeuc_000\\Documents\\GitHub\\gather\\temp_history.txt", "rb");
	if (error != 0) {
		return ;
	}
	//	OutputDebugString(L"==== Loadtext start ====");
	if (gs_text.capacity() == 0) {
		gs_text.reserve(1024);
	}
	WCHAR line[64 * 1024];
	while (true) {
		if (fgetws(line, _countof(line), fp) == 0) {
			break;
		}
		//OutputDebugString(line);
		gs_text.push_back(line);
	}

	fclose(fp);
	std::sort(gs_text.begin(), gs_text.end());
	//OutputDebugString(L"==== Loadtext finish ====");	
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

/*
search_words	スペース区切りの検索文字列
				(Ex.) "c: programs .txt"
返値		マッチした文字列
*/
extern "C" WCHAR* Filter(WCHAR* search_words) {
	//OutputDebugString(L"====Enter====");
	//OutputDebugString(search_words);
	LoadTextFile();
	
	gs_result_text.reserve(gs_text.size());
	gs_result_text.clear();

	std::vector<std::wstring> tokens;
	tokens.reserve(16);
	Tokenizer(tokens, search_words);
	//std::sort(token.begin(), token.end());

	//OutputDebugString(L"==== Find start ====");
	if (tokens.size()==0) {
		//全ての行を返す
		const size_t size = gs_text.size();
		for (size_t i = 0; i < size; ++i) {
			const std::wstring& line = gs_text.at(i);
			gs_result_text.insert(gs_result_text.end(), line.begin(), line.end());
		}
	}else{
		//一致する行を返す
		const size_t size = gs_text.size();
		for (size_t i = 0; i < size; ++i) {
			const std::wstring& line = gs_text.at(i);
			if (MatchAll(line, tokens)) {
				gs_result_text.insert(gs_result_text.end(), line.begin(), line.end());
			}
		}
	}
	//OutputDebugString(L"==== Find finish ====");
	if (gs_result_text.size() == 0) {
		return gs_empty;
	}else {
		gs_result_text.push_back(L'\0');
	}

//	OutputDebugString(L"==== Result start ====");
	//OutputDebugString(&gs_result_text[0]);	
	//OutputDebugString(L"==== Result finish ====");
	return &gs_result_text[0];
}