#pragma once
#include"define.h"

/* データ構造

＊候補リスト(candidate list)
	┌───┬───────────┐
	│ index│  候補の内容          │
	├───┼───────────┤
	│  0   │  候補A               │
	│      │      サブ候補A1      │
	│      │      サブ候補A2      │
	├───┼───────────┤
	│  1   │  候補B               │
	│      │      サブ候補B1      │
	├───┼───────────┤
	│  2   │  候補C               │
	│      │      サブ候補C1      │
	├───┼───────────┤
	│  3   │  候補D               │
	└───┴───────────┘
	＊対応するメンバ変数
	  Candidates::m_candidates


＊秀丸エディタの表示
	┌────┬───────┬───────────┬───────────────┐
	│  行番号│候補          │候補リストインデックス│折りたたんだ候補のインデックス│
	├────┼───────┼───────────┼───────────────┤
	│  1     │候補B         │    1                 │    0                         │
	├────┼───────┼───────────┼───────────────┤
	│  2     │    サブ候補B1│    1                 │    0                         │
	├────┼───────┼───────────┼───────────────┤
	│  3     │候補D         │    3                 │    1                         │
	└────┴───────┴───────────┴───────────────┘
	＊対応するメンバ変数
	  HidemaruView::m_hidemaru_lineno_to_candidate_list_index


＊折りたたんだ候補リスト
	┌────┬─────────┬──────────┐
	│  index │候補インデックス  │秀丸エディタの行番号│
	├────┼─────────┼──────────┤
	│  0     │1                 │1                   │
	├────┼─────────┼──────────┤
	│  1     │3                 │3                   │
	└────┴─────────┴──────────┘
	＊対応するメンバ変数
	　HidemaruView::CollapsedCandidate::m_collapsed_index_to_hidemaru_lineno
	　HidemaruView::CollapsedCandidate::m_hidemaru_line_index_to_collapsed_index
*/


class Unity;


/*子供を折りたたんだ候補*/
struct CollapsedCandidate {
private:
	Unity*				m_instance;
public:
	CollapsedCandidate(Unity *instance);

	///候補インデックスから秀丸エディタの行番号へ変換するテーブル
	std::vector<INT_PTR>	m_collapsed_index_to_hidemaru_lineno;
	///秀丸エディタの行インデックスから候補インデックスへ変換するテーブル
	std::vector<INT_PTR>	m_hidemaru_line_index_to_collapsed_index;
	
	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_collapsed_index_to_hidemaru_lineno,
			m_hidemaru_line_index_to_collapsed_index
		);
	};

	void Clear();
	void Reserve(size_t size);
	void OnChangeHidemaruLineNo(INT_PTR hidemaru_lineno, INT_PTR collapsed_index);
	void OnChangeCollapsedIndex(INT_PTR hidemaru_lineno, INT_PTR collapsed_index);
	INT_PTR CalcHidemaruCursorLineNo(INT_PTR hidemaru_lineno, INT_PTR collapsed_delta);
	
};

//秀丸エディタへ返す情報
struct HidemaruView {
	HidemaruView(Unity *instance): m_collapsed(instance){

	};

	void Clear() {
		m_hidemaru_text.clear();
		m_hidemaru_line_index_to_candidate_index.clear();
		m_hidemaru_maeked_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_hidemaru_text.reserve(size*text_line_char);
		m_hidemaru_line_index_to_candidate_index.reserve(size);
		m_hidemaru_maeked_lineno.reserve(size);
	};

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_hidemaru_text,
			m_hidemaru_line_index_to_candidate_index,
			m_hidemaru_maeked_lineno,
			m_collapsed
		);
	};

	
	CollapsedCandidate		m_collapsed;

	//秀丸エディタへ返す文字列(Ex. "foo.txt\nbar.txt\nhoge.cpp")
	std::vector<std::wstring::value_type>	m_hidemaru_text;

	/*
	「秀丸エディタの行インデックス」から「候補リストのインデックス」を取得するテーブル

	(使用例)
	候補のインデックス = m_hidemaru_lineno_to_candidate_list_index[秀丸エディタの行番号-1];
	*/
	std::vector<INT_PTR>					m_hidemaru_line_index_to_candidate_index;

	//秀丸エディタでマークしている行番号(インデックスは1始まり)
	std::vector<INT_PTR>					m_hidemaru_maeked_lineno;
};




class RefineSearch {
public:
	RefineSearch(Unity* instance);
	bool Do(const WCHAR* search_words);
	WCHAR* GetResult();

	/*秀丸エディタの行番号を設定する
	*/
	void SetHidemaruLineno(INT_PTR hidemaru_line_no);

	////////////////////////////////////////////////////////////////////////////
	//変換
	////////////////////////////////////////////////////////////////////////////
	/**秀丸エディタの行番号から候補リストのインデックスを取得する
	return	成功	0以上の数値
			失敗	-1
	*/
	INT_PTR ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no);


	////////////////////////////////////////////////////////////////////////////
	//マーク
	////////////////////////////////////////////////////////////////////////////
	/*行を「マークする・マーク解除する」
	hidemaru_line_no	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
	　					ファイルの先頭が１です。
	*/
	INT_PTR ChangeMarked(INT_PTR hidemaru_line_no, bool is_selected);


	/*マークされているファイル名を取得する（秀丸エディタの行番号バージョン）
	return	成功	ファイル名
			失敗	空文字
	*/
	//WCHAR* GetMarkedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no);

	/*マークされた行数を取得する
	*/
	INT_PTR GetMarkedCount();

	/*マークのインデックスから秀丸エディタの行番号へ変換する
	return	秀丸エディタの行番号
	*/
	INT_PTR ConvertSelectedIndexToHidemaruLineno(INT_PTR marked_index);

	/*マークインデックスから候補インデックスへ変換する
	*/
	INT_PTR ConvertMarkIndexToCandidatesIndex(INT_PTR marked_index);

	/*マークしたファイル名を取得する
	*/
	//WCHAR* GetSelectedFilename(INT_PTR marked_index);

	/*マークした候補の情報を取得する
	*/
	//Candidate* GetMarkedCandidate(INT_PTR marked_index);

	////////////////////////////////////////////////////////////////////////////
	//選択
	////////////////////////////////////////////////////////////////////////////

	/** 最初に選択した候補のインデックスを取得する
	return	not -1	候補インデックス
			-1		無し
	*/
	INT_PTR	GetFirstSelectionCandidateIndex();

	/** 選択した候補数を取得する
	*/
	INT_PTR	GetSelectionCandidateCount();

	/** 選択した候補のインデックスを取得する
	*/
	INT_PTR	GetSelectionCandidateIndex(INT_PTR selected_index);

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_hidemaru_view,
			m_hidemaru_line_no
		);
	};

	/**秀丸エディタのカーソルを上下に移動する
	candidate_delta	候補の増分
	*/
	INT_PTR MoveHidemaruCursorLineNo(INT_PTR current_line_no, INT_PTR candidate_delta);

private:
	Unity*				m_instance;
	///検索結果
	HidemaruView				m_hidemaru_view;
	///秀丸エディタの行番号
	INT_PTR				m_hidemaru_line_no;
};