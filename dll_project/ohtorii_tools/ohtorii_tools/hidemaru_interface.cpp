﻿#include "stdafx.h"



/////////////////////////////////////////////////////////////////////////////
//絞り込み検索
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR Push() {
	return Unity::Push();
}

extern "C" INT_PTR Pop() {
	return Unity::Pop();
}

extern "C" INT_PTR GetCurrentContext() {
	return Unity::GetCurrentInstanceIndex();
}

extern "C" INT_PTR ChangeSelected(INT_PTR hidemaru_line_no, INT_PTR is_selected) {	
	return Unity::Instance()->QueryRefineSearch()->ChangeSelected(hidemaru_line_no, is_selected);
}

extern "C" INT_PTR GetSelectionCount() {
	return Unity::Instance()->QueryRefineSearch()->GetSelectionCount();
}

extern "C" INT_PTR RefineSearchConvertSelectedIndexToHidemaruLineno(INT_PTR index) {
	return Unity::Instance()->QueryRefineSearch()->ConvertSelectedIndexToHidemaruLineno(index);
}

extern "C" INT_PTR RefineSearchConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no) {
	return Unity::Instance()->QueryRefineSearch()->ConvertHidemaruLinenNoToCandidateIndex(hidemaru_line_no);
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

extern "C" WCHAR* RefineSearchGetResult() {
	return Unity::Instance()->QueryRefineSearch()->GetResult();
}

extern "C" INT_PTR GenerateKindCandidates(INT_PTR instance_index) {
	return Unity::Instance()->QueryKinds()->GenerateKindCandidates(instance_index);
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
	auto*source = Unity::Instance()->QuerySources()->FindSource(souce_name);
	if(source==nullptr){
		return _T("");
	}
	return source->m_candidate_type.c_str();
}

extern "C" const WCHAR* SourcesGetDefaultKind(WCHAR*souce_name) {
	auto*source = Unity::Instance()->QuerySources()->FindSource(souce_name);
	if (source == nullptr) {
		return _T("");
	}
	return source->m_default_kind.c_str();
}

/////////////////////////////////////////////////////////////////////////////
//候補
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR CandidatesAppendW(WCHAR*source_name, WCHAR*candidate, const WCHAR*user_data){
	return Unity::Instance()->QueryCandidates()->AppendCandidate(source_name, candidate, user_data);
}

extern "C" INT_PTR CandidatesAppend(WCHAR*source_name, WCHAR*candidate) {
	return Unity::Instance()->QueryCandidates()->AppendCandidate(source_name, candidate);
}

extern "C" WCHAR* CandidatesGetSourceName(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance()->QueryCandidates()->GetSourceName(index));
}

extern "C" WCHAR* CandidatesGetText(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance()->QueryCandidates()->GetText(index));
}

extern "C" WCHAR* CandidatesGetUserData(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance()->QueryCandidates()->GetUserData(index));
}

/////////////////////////////////////////////////////////////////////////////
//kind
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR KindsClear() {
	Unity::Instance()->QueryKinds()->Clear();
	return true;
}

extern "C" WCHAR* KindsCreate(WCHAR* kind_ini) {
	return Unity::Instance()->QueryKinds()->Create(kind_ini);
}

extern "C" WCHAR* KindsGetHidemaruLabelName(WCHAR* kind_name) {
	return const_cast<WCHAR*>(Unity::Instance()->QueryKinds()->GetHidemaruLabelName(kind_name));
}

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
