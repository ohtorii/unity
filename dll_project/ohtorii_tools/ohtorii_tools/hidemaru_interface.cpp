#include "stdafx.h"



/////////////////////////////////////////////////////////////////////////////
//絞り込み検索
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected) {	
	return Unity::Instance()->QueryRefineSearch()->ChangeSelected(hidemaru_line_no, is_selected);
}

extern "C" INT_PTR GetSelectionCount() {
	return Unity::Instance()->QueryRefineSearch()->GetSelectionCount();
}

extern "C" INT_PTR GetSelectedLineno(INT_PTR index) {
	return Unity::Instance()->QueryRefineSearch()->GetSelectedLineno(index);
}

extern "C" WCHAR* GetSelectedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no) {
	return Unity::Instance()->QueryRefineSearch()->GetSelectedFilenameFromHidemaruLineNo(hidemaru_line_no);
}

extern "C" WCHAR* GetSelectedFilename(INT_PTR index) {
	return Unity::Instance()->QueryRefineSearch()->GetSelectedFilename(index);
}

extern "C" INT_PTR RefineSearch(WCHAR* search_words) {
	return Unity::Instance()->QueryRefineSearch()->Do(search_words);
}

extern "C" WCHAR* GetRefineSearchResult() {
	return Unity::Instance()->QueryRefineSearch()->GetResult();
}

/////////////////////////////////////////////////////////////////////////////
//source
/////////////////////////////////////////////////////////////////////////////
//廃止
/*extern "C" INT_PTR SetCandidateList(WCHAR* source_filename, WCHAR*source_name, WCHAR*source_description) {
	return Unity::Instance()->SetCandidateList(source_filename, source_name, source_description);
}*/

extern "C" WCHAR* SourcesCreate(WCHAR* source_ini){
	return Unity::Instance()->QuerySources()->Create(source_ini);
}

extern "C" const WCHAR* SourcesGetCandidateType(WCHAR*souce_name) {
	return Unity::Instance()->QuerySources()->GetCandidateType(souce_name);
}


/////////////////////////////////////////////////////////////////////////////
//候補
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR SourcesAppendCandidateW(WCHAR*source_name, WCHAR*candidate, const WCHAR*user_data){
	return Unity::Instance()->QueryCandidates()->AppendCandidate(source_name, candidate, user_data);
}

extern "C" INT_PTR SourcesAppendCandidate(WCHAR*source_name, WCHAR*candidate) {
	return Unity::Instance()->QueryCandidates()->AppendCandidate(source_name, candidate);
}


/////////////////////////////////////////////////////////////////////////////
//action
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR WriteToFile(const WCHAR* filename, const WCHAR* string) {
	return Unity::Instance()->QueryFile()->WriteToFile(filename, string);
}

extern "C" INT_PTR DllDetachFunc_After_Hm866( INT_PTR n  ) {
	/*ここで一時ファイルを削除する*/
	Unity::Destroy();
	return 0;
}
