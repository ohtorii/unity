#pragma once
#include<vector>
#include<string>
#include"sources.h"
#include"file.h"
#include"candidates.h"


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

	//秀丸エディタで選択する行番号(インデックスは1始まり)
	std::vector<__int64>					m_selected_lineno;
};



class Unity{
public:
	Unity();
	~Unity();

	static Unity* Instance();
	
	/*行を「選択・選択解除」する
	hidemaru_line_no	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
	　					ファイルの先頭が１です。
	*/
	INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected);
	
	/*選択行の個数を取得する
	*/
	INT_PTR GetSelectionCount();

	/*選択行を取得する
	return	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
	　		ファイルの先頭が１です。
	*/
	INT_PTR GetSelectedLineno(INT_PTR index);

	/*選択されているファイル名を取得する（秀丸エディタの行番号バージョン）
	*/
	WCHAR* GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no);
	
	/*選択されているファイル名を取得する
	*/
	WCHAR* GetSelectedFilename(INT_PTR index);

	/*ファイルリストのファイル名を設定する
	return	bool	true	成功
					false	失敗
	*/
	INT_PTR SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description);
	
	/*
	search_words	スペース区切りの検索文字列
					(Ex.) "c: programs .txt"
	返値		マッチした文字列
	*/
	WCHAR* Filter(WCHAR* search_words);
	
	void AppendAfterDelete(const WCHAR*filename){}
	Sources* 	QuerySources();
	File*		QueryFile();
	Candidates*	QueryCandidates();
protected:
private:
	Unity(const Unity&);
	Unity &operator=(const Unity&);
	
	static Unity*		m_instance;
	Sources				m_sources;
	File				m_file;
	Candidates			m_candidates;

	std::vector<Candidate>	m_input;
	Output				m_output;
	
	
	bool LoadTextFile(const WCHAR*filename);
	
};
