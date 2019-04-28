#pragma once
#include"define.h"


//秀丸エディタへ返す情報
struct Output {
	void Clear() {
		m_text.clear();
		m_hidemaru_lineno_to_candidate_index.clear();
		m_hidemaru_maeked_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_text.reserve(size*text_line_char);
		m_hidemaru_lineno_to_candidate_index.reserve(size);
		m_hidemaru_maeked_lineno.reserve(size);
	};

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_text,
			m_hidemaru_lineno_to_candidate_index,
			m_hidemaru_maeked_lineno
		);
	};

	//秀丸エディタへ返す文字列(Ex. "foo.txt\nbar.txt\nhoge.cpp")
	std::vector<std::wstring::value_type>	m_text;

	/*「秀丸エディタの行番号」から「候補リストのインデックス」を取得するテーブル

	(使用例)
	候補のインデックス = m_hidemaru_lineno_to_candidate_index[秀丸エディタの行番号];
	*/
	std::vector<INT_PTR>					m_hidemaru_lineno_to_candidate_index;

	//秀丸エディタでマークしている行番号(インデックスは1始まり)
	std::vector<INT_PTR>					m_hidemaru_maeked_lineno;
};


class Unity;

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
	Candidate* GetMarkedCandidate(INT_PTR marked_index);	

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
			m_output, 
			m_hidemaru_line_no
		);
	};

private:
	Unity*				m_instance;
	///検索結果
	Output				m_output;
	///秀丸エディタの行番号
	INT_PTR				m_hidemaru_line_no;

	void Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates);
};