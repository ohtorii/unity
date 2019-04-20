#pragma once
#include<vector>
#include<string>
#include<array>
#include"define.h"
#include"sources.h"
#include"file.h"
#include"candidates.h"
#include"refine_search.h"
#include"kinds.h"



class Unity{
public:
	Unity();
	~Unity();

	static Unity* Instance(size_t index= UNITY_USE_URRENT_INSTANCE);
	static bool Push();
	static bool Pop();
	static size_t GetCurrentInstanceIndex();
	static void Destroy();
	
	/*ファイルリストのファイル名を設定する
	return	bool	true	成功
					false	失敗
	*/
	//INT_PTR SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description);
	
	/*
	search_words	スペース区切りの検索文字列
					(Ex.) "c: programs .txt"
	返値		マッチした文字列
	*/
	//WCHAR* Filter(WCHAR* search_words);
	
	void AppendAfterDelete(const WCHAR*filename){}
	
	Sources* 		QuerySources();
	File*			QueryFile();
	Candidates*		QueryCandidates();
	RefineSearch*	QueryRefineSearch();
	Kinds*			QueryKinds();


protected:
private:
	Unity(const Unity&);
	Unity &operator=(const Unity&);


	static	std::array<Unity*,4>	m_instances;
	static	size_t					m_current_instance_index;

	Sources				m_sources;
	File				m_file;
	Candidates			m_candidates;
	RefineSearch		m_refine_search;
	///カインドはインスタンス共通で利用するため静的領域とする
	static Kinds		m_kinds;

	//std::vector<Candidate>	m_input;
	//Output				m_output;
	
	
	//bool LoadTextFile(const WCHAR*filename);
	
};
