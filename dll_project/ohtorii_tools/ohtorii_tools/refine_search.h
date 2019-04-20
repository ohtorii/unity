#pragma once
#include"define.h"


//秀丸エディタへ返す情報
struct Output {
	void Clear() {
		m_text.clear();
		m_hidemaru_lineno_to_candidate_index.clear();
		m_hidemaru_selected_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_text.reserve(size*text_line_char);
		m_hidemaru_lineno_to_candidate_index.reserve(size);
		m_hidemaru_selected_lineno.reserve(size);
	};

	//秀丸エディタへ返す文字列(Ex. "foo.txt\nbar.txt\nhoge.cpp")
	std::vector<std::wstring::value_type>	m_text;

	/*「秀丸エディタの行番号」から「候補リストのインデックス」を取得するテーブル

	(使用例)
	候補のインデックス = m_hidemaru_lineno_to_candidate_index[秀丸エディタの行番号];
	*/
	std::vector<__int64>					m_hidemaru_lineno_to_candidate_index;

	//秀丸エディタで選択している行番号(インデックスは1始まり)
	std::vector<__int64>					m_hidemaru_selected_lineno;
};


class Unity;

class RefineSearch {
public:
	RefineSearch(Unity*instance) : m_instance(instance) {
	}
	bool Do(const WCHAR* search_words);
	WCHAR* GetResult();

	/*行を「選択・選択解除」する
	hidemaru_line_no	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
	　					ファイルの先頭が１です。
	*/
	INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, bool is_selected);
	
	
	/*選択されているファイル名を取得する（秀丸エディタの行番号バージョン）
	return	成功	ファイル名
			失敗	空文字
	*/
	WCHAR* GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no);

	/**秀丸エディタの行番号から候補リストのインデックスを取得する
	return	成功	0以上の数値
			失敗	-1
	*/
	INT_PTR ConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no);

	/*選択行の個数を取得する
	*/
	INT_PTR GetSelectionCount();

	/*選択行を取得する
	return	秀丸エディタのカーソル位置の、エディタ的に計算した行番号です。
	　		ファイルの先頭が１です。
	*/
	INT_PTR ConvertSelectedIndexToHidemaruLineno(INT_PTR selected_index);

	/*選択されているファイル名を取得する	
	*/
	WCHAR* GetSelectedFilename(INT_PTR seleted_index);

	/*選択されている候補の情報を取得する	
	*/
	Candidate* GetSelectedCandidate(INT_PTR seleted_index);

private:
	Unity*				m_instance;
	///検索結果
	Output				m_output;

	void Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates);
};