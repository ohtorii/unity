#pragma once
#include"define.h"


//秀丸エディタへ返す情報
struct HidemaruView {
	void Clear() {
		m_hidemaru_text.clear();
		m_hidemaru_lineno_to_candidate_list_index.clear();		
		m_hidemaru_maeked_lineno.clear();
	};

	void Reserve(size_t size) {
		const size_t text_line_char = 80;
		m_hidemaru_text.reserve(size*text_line_char);
		m_hidemaru_lineno_to_candidate_list_index.reserve(size);		
		m_hidemaru_maeked_lineno.reserve(size);
	};

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_hidemaru_text,
			m_hidemaru_lineno_to_candidate_list_index,	
			m_hidemaru_maeked_lineno,
			m_collapsed
		);
	};
	
	/*子供を折りたたんだ候補*/
	struct CollapsedCandidate {
		std::vector<INT_PTR>	m_index_to_hidemaru_lineno;
		std::vector<INT_PTR>	m_hidemaru_lineno_to_index;
		
		void Clear() {
			m_index_to_hidemaru_lineno.clear();
			m_hidemaru_lineno_to_index.clear();
		}

		void Reserve(size_t size) {
			m_index_to_hidemaru_lineno.reserve(size);
			m_hidemaru_lineno_to_index.reserve(size);
		}

		template<class Archive> void serialize(Archive & archive) {
			archive(
				m_index_to_hidemaru_lineno,
				m_hidemaru_lineno_to_index
			);
		};

		void OnChangeHidemaruLineNo(INT_PTR hidemaru_lineno, INT_PTR collapsed_index) {
			m_hidemaru_lineno_to_index.push_back(collapsed_index);
		}

		void OnChangeCollapsedIndex(INT_PTR hidemaru_lineno,INT_PTR collapsed_index) {
			m_index_to_hidemaru_lineno.push_back(hidemaru_lineno);
		}

		INT_PTR CalcHidemaruCursorLineNo(INT_PTR hidemaru_lineno, INT_PTR candidate_delta) {
			auto hidemar_line_index = hidemaru_lineno - 1;	//0開始にする
			try{
				auto next = m_hidemaru_lineno_to_index.at(hidemar_line_index) + candidate_delta;
				return m_index_to_hidemaru_lineno.at(next);
			}
			catch (std::exception) {
				//pass
			}
			return UNITY_NOT_FOUND_INDEX;
		}
	};
	CollapsedCandidate		m_collapsed;

	//秀丸エディタへ返す文字列(Ex. "foo.txt\nbar.txt\nhoge.cpp")
	std::vector<std::wstring::value_type>	m_hidemaru_text;	

	/*	廃止予定
	
	「秀丸エディタの行番号」から「候補リストのインデックス」を取得するテーブル

	(使用例)
	候補のインデックス = m_hidemaru_lineno_to_candidate_list_index[秀丸エディタの行番号];
	*/
	std::vector<INT_PTR>					m_hidemaru_lineno_to_candidate_list_index;
		
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

	void Filter(const std::vector<std::wstring> &tokens, const std::vector<Candidate>&candidates);
};