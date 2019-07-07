#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//Source制作者が利用する関数
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR SetCurrentSourceName(WCHAR* source_name) {
	return InterfaceSugar::Instance().SetCurrenSourceName(source_name);
}

extern "C" INT_PTR ClearCurrentSourceName() {
	return InterfaceSugar::Instance().ClearCurrentSourceName();
}

extern "C" INT_PTR AppendCandidateHeader(WCHAR*header, WCHAR*description) {
	return InterfaceSugar::Instance().AppendCandidateHeader(header, description);
}

extern "C" INT_PTR AppendCandidate(WCHAR*candidate, WCHAR*description) {
	return InterfaceSugar::Instance().AppendCandidate(candidate, description);
}

extern "C" INT_PTR AppendUserDataString(const WCHAR* key, WCHAR* data) {
	return InterfaceSugar::Instance().AppendUserDataString(key, data);
}

extern "C" INT_PTR AppendUserDataNumeric(const WCHAR* key, INT_PTR data) {
	return InterfaceSugar::Instance().AppendUserDataNumeric(key,data);
}

extern "C" INT_PTR AppendChildCandidate(WCHAR*candidate, WCHAR*description) {
	return InterfaceSugar::Instance().AppendChildCandidate(candidate, description);
}

/////////////////////////////////////////////////////////////////////////////
//Kind制作者が利用する関数
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR GetSelectionCount() {
	return Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateCount();
}

extern "C" WCHAR* GetSelectionText(INT_PTR selected_index) {
	auto index=Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetText(index));
}

extern "C" WCHAR* GetSelectionSourceName(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetSourceName(index));
}

extern "C" WCHAR* GetSelectionDescription(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetDescription(index));
}

extern "C" WCHAR* GetSelectionUserDataString(INT_PTR selected_index, WCHAR*key_name, WCHAR*default_value) {
	auto index = Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetUserData(index,key_name, default_value));
}

extern "C" INT_PTR GetSelectionUserDataNumeric(INT_PTR selected_index, WCHAR*key_name, INT_PTR default_value) {
	auto index = Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates()->GetUserData(index, key_name, default_value);
}

/////////////////////////////////////////////////////////////////////////////
//Unity
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR Initialize() {
	/*_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	OutputDebugString(_T("call Initialize()"));*/
	return true;
}

extern "C" INT_PTR PushContext(INT_PTR exist_context_then_delete) {
	return Unity::PushContext(exist_context_then_delete);
}

extern "C" INT_PTR PopContext(INT_PTR exist_context_then_delete) {
	return Unity::PopContext(exist_context_then_delete);
}

extern "C" INT_PTR GetCurrentContext() {
	return Unity::GetCurrentInstanceIndex();
}

extern "C" INT_PTR SerializeCurrentContext(WCHAR* out_filename) {
	return Unity::Instance().lock()->SerializeCurrentContext(out_filename);
}

extern "C" INT_PTR DeSerializeToCurrentContext(WCHAR* input_filename) {
	return Unity::Instance().lock()->DeSerializeToCurrentContext(input_filename);
}

/////////////////////////////////////////////////////////////////////////////
//絞り込み検索
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR RefineSearchChangeMarked(INT_PTR hidemaru_line_no, INT_PTR is_selected) {	
	return Unity::Instance().lock()->QueryRefineSearch()->ChangeMarked(hidemaru_line_no, is_selected);
}

extern "C" INT_PTR RefineSearchGetMarkedCount() {
	return Unity::Instance().lock()->QueryRefineSearch()->GetMarkedCount();
}

extern "C" INT_PTR RefineSearchConvertSelectedIndexToHidemaruLineno(INT_PTR index) {
	return Unity::Instance().lock()->QueryRefineSearch()->ConvertSelectedIndexToHidemaruLineno(index);
}

extern "C" INT_PTR RefineSearchConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no) {
	return Unity::Instance().lock()->QueryRefineSearch()->ConvertHidemaruLinenNoToCandidateIndex(hidemaru_line_no);
}

/*extern "C" WCHAR* GetMarkedFilenameFromHidemaruLineNo(INT_PTR hidemaru_line_no) {
	return Unity::Instance().QueryRefineSearch()->GetMarkedFilenameFromHidemaruLineNo(hidemaru_line_no);
}*/

/*extern "C" WCHAR* GetSelectedFilename(INT_PTR index) {
	return Unity::Instance().QueryRefineSearch()->GetSelectedFilename(index);
}*/

extern "C" INT_PTR RefineSearchSetHidemaruLineno(INT_PTR hidemaru_line_no) {
	Unity::Instance().lock()->QueryRefineSearch()->SetHidemaruLineno(hidemaru_line_no);
	return true;
}

extern "C" INT_PTR RefineSearchDo(WCHAR* search_words) {
	return Unity::Instance().lock()->QueryRefineSearch()->Do(search_words);
}

extern "C" WCHAR* RefineSearchGetResult() {
	return Unity::Instance().lock()->QueryRefineSearch()->GetResult();
}

extern "C" INT_PTR RefineSearchGetFirstSelectionCandidateIndex() {
	return Unity::Instance().lock()->QueryRefineSearch()->GetFirstSelectionCandidateIndex();
}

extern "C" INT_PTR	RefineSearchGetSelectionCandidateCount() {
	return GetSelectionCount();
	//return Unity::Instance().QueryRefineSearch()->GetSelectionCandidateCount();
}

extern "C" INT_PTR	RefineSearchGetSelectionCandidateIndex(INT_PTR selected_index) {
	return Unity::Instance().lock()->QueryRefineSearch()->GetSelectionCandidateIndex(selected_index);
}

extern "C" INT_PTR	RefineSearchMoveHidemaruCursorLineNo(INT_PTR current_line_no, INT_PTR delta) {
	return Unity::Instance().lock()->QueryRefineSearch()->MoveHidemaruCursorLineNo(current_line_no, delta);
}

/////////////////////////////////////////////////////////////////////////////
//source
/////////////////////////////////////////////////////////////////////////////
//廃止
/*extern "C" INT_PTR SetCandidateList(WCHAR* source_filename, WCHAR*source_name, WCHAR*source_description) {
	return Unity::Instance().SetCandidateList(source_filename, source_name, source_description);
}*/

extern "C" WCHAR* SourcesCreate(WCHAR* source_ini){
	return Unity::Instance().lock()->QuerySources()->Create(source_ini);
}

extern "C" const WCHAR* SourcesGetCandidateType(WCHAR*souce_name) {
	auto*source = Unity::Instance().lock()->QuerySources()->FindSource(souce_name);
	if(source==nullptr){
		return _T("");
	}
	return source->m_candidate_type.c_str();
}

extern "C" const WCHAR* SourcesGetDefaultKind(WCHAR*souce_name) {
	auto*source = Unity::Instance().lock()->QuerySources()->FindSource(souce_name);
	if (source == nullptr) {
		return _T("");
	}
	return source->m_default_kind.c_str();
}

/////////////////////////////////////////////////////////////////////////////
//候補
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR CandidatesAppendHeader(WCHAR*source_name, WCHAR*header, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates()->AppendCandidateHeader(source_name, header, description);
}

extern "C" INT_PTR CandidatesAppend(WCHAR*source_name, WCHAR*candidate, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates()->AppendCandidate(source_name, candidate, description);
}

extern "C" INT_PTR CandidatesAppendChild(INT_PTR candidate_index, WCHAR*candidate, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates()->AppendChildCandidate(candidate_index, candidate, description);
}

extern "C" INT_PTR CandidatesSetUserDataString(INT_PTR index, const WCHAR* key, const WCHAR*data) {
	return Unity::Instance().lock()->QueryCandidates()->SetUserData(index, key,data);
}

extern "C" INT_PTR CandidatesSetUserDataNumeric(INT_PTR index, const WCHAR* key, INT_PTR data) {
	return Unity::Instance().lock()->QueryCandidates()->SetUserData(index, key, data);
}

extern "C" const WCHAR*	CandidatesGetUserDataString(INT_PTR index, const WCHAR* key, const WCHAR*	default_data) {
	return Unity::Instance().lock()->QueryCandidates()->GetUserData(index, key, default_data);
}

extern "C" INT_PTR		CandidatesGetUserDataNumeric(INT_PTR index, const WCHAR* key, INT_PTR default_data) {
	return Unity::Instance().lock()->QueryCandidates()->GetUserData(index,key,default_data);
}

extern "C" WCHAR* CandidatesGetSourceName(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetSourceName(index));
}

extern "C" WCHAR* CandidatesGetText(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates()->GetText(index));
}

/////////////////////////////////////////////////////////////////////////////
//kind
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR KindsClear() {
	Unity::Instance().lock()->QueryKinds()->Clear();
	return true;
}

extern "C" WCHAR* KindsCreate(WCHAR* kind_ini) {
	return Unity::Instance().lock()->QueryKinds()->Create(kind_ini);
}

extern "C" INT_PTR KindsGenerateCandidates(INT_PTR instance_index) {
	return Unity::Instance().lock()->QueryKinds()->GenerateKindCandidates(instance_index);
}

extern "C" WCHAR* KindsGetDefaultActionLabelName(WCHAR* kind_name) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetDefaultActionLabelName(kind_name));
}

extern "C" INT_PTR	KindsFindKindIndex(const WCHAR* kind_name) {
	return Unity::Instance().lock()->QueryKinds()->FindKindIndex(kind_name);
}

extern "C" INT_PTR	KindsFindActionIndex(INT_PTR kind_index, const WCHAR* action_name) {
	return Unity::Instance().lock()->QueryKinds()->FindActionIndex(kind_index, action_name);
}

//
// Kind
// 
extern "C" WCHAR* KindGetKindName(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetKindName(kind_index));
}

extern "C" WCHAR* KindGetKindDescription(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetKindDescription(kind_index));
}

extern "C" WCHAR* KindGetDefaultAction(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetDefaultAction(kind_index));
}

//
// Action
// 
extern "C" WCHAR*	KindsGetActionName(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetActionName(kind_index, action_index));
}

extern "C" WCHAR*	KindsGetActionLabelName(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetActionLabelName(kind_index, action_index));
}

extern "C" WCHAR*	KindsGetActionDescription(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds()->GetActionDescription(kind_index, action_index));
}

extern "C" INT_PTR	KindsIsActionQuit(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds()->IsActionQuit(kind_index, action_index);
}

extern "C" INT_PTR	KindsIsActionMultiSelectable(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds()->IsActionMultiSelectable(kind_index, action_index);
}

/////////////////////////////////////////////////////////////////////////////
//Inheritance
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR InheriatnceGenerateDefaultAction(const WCHAR*source_name) {
	return Unity::Instance().lock()->QueryInheritance()->GenerateDefaultAction(source_name);
}

extern "C" WCHAR*	InheriatnceGetDefaultActionKind() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryInheritance()->GetDefaultAction().m_kind_name.c_str());
}

extern "C" WCHAR*	InheriatnceGetDefaultActionLabel() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryInheritance()->GetDefaultAction().m_label_name.c_str());
}

/////////////////////////////////////////////////////////////////////////////
//file
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR WriteToFile(const WCHAR* filename, const WCHAR* string) {
	return Unity::Instance().lock()->QueryFile()->WriteToFile(filename, string);
}

extern "C" INT_PTR FileRegistAfterDeleteFile(const WCHAR* filename) {
	Unity::Instance().lock()->QueryFile()->RegistAfterDelete(filename);
	return true;
}

extern "C" INT_PTR DllDetachFunc_After_Hm866( INT_PTR n  ) {	
	/*Todo ここで一時ファイルを削除する*/
	Unity::Destroy();

	/*_ASSERTE(_CrtCheckMemory());
	DebugLog(_T("DllDetachFunc_After_Hm866 (%d)"), n);*/
	return 0;
}
