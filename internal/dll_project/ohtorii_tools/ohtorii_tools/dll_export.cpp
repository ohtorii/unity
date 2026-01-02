#include "stdafx.h"


struct PathPointers {
	void Destroy() {
		m_path_split.clear();
		m_basename.clear();
		m_source_created.clear();
	};
	std::wstring m_path_split;
	std::wstring m_basename;
	std::wstring m_source_created;
};
static PathPointers gs_path_pointers;



/////////////////////////////////////////////////////////////////////////////
//Source制作者が利用する関数
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR BeginSource(WCHAR* source_name) {
	return InterfaceSugar::Instance().BeginSource(source_name);
}

extern "C" INT_PTR EndSource() {
	return InterfaceSugar::Instance().EndSource();
}

extern "C" INT_PTR AppendCandidateHeader(WCHAR*header, WCHAR*description) {
	return InterfaceSugar::Instance().AppendCandidateHeader(header, description);
}

extern "C" INT_PTR AppendCandidate(WCHAR*candidate, WCHAR*description) {
	return InterfaceSugar::Instance().AppendCandidate(candidate, description);
}

extern "C" INT_PTR AppendCandidateFix(WCHAR*prefix, WCHAR*candidate, WCHAR*postfix, WCHAR*description) {
	return InterfaceSugar::Instance().AppendCandidateFix(prefix,candidate, postfix,description);
}

extern "C" INT_PTR AppendCandidateAsASyncFile(WCHAR*filename) {
	return InterfaceSugar::Instance().AppendCandidateAsASyncFile(filename);
}
extern "C" INT_PTR AppendCandidateAsString(WCHAR *string) {
	return InterfaceSugar::Instance().AppendCandidateAsString(string);
}

extern "C" INT_PTR SetCandidateActionDirectoryName(WCHAR*directory_name) {
	return InterfaceSugar::Instance().SetCandidateActionDirectoryName(directory_name);
}

extern "C" INT_PTR SetCandidateActionFileName(WCHAR*filename) {
	return InterfaceSugar::Instance().SetCandidateActionFileName(filename);
}

extern "C" INT_PTR SetCandidateActionColumn(INT_PTR column) {
	return InterfaceSugar::Instance().SetCandidateActionColumn(column);
}

extern "C" INT_PTR SetCandidateActionLine(INT_PTR line) {
	return InterfaceSugar::Instance().SetCandidateActionLine(line);
}

extern "C" INT_PTR SetCandidateActionCommand(const WCHAR*command) {
	return InterfaceSugar::Instance().SetCandidateActionCommand(command);
}

extern "C" INT_PTR SetCandidateActionProcessId(INT_PTR process_id) {
	return InterfaceSugar::Instance().SetCandidateActionProcessId(process_id);
}

extern "C" INT_PTR SetCandidateActionWindowHandle(INT_PTR window_handle) {
	return InterfaceSugar::Instance().SetCandidateActionWindowHandle(window_handle);
}

extern "C" INT_PTR SetCandidateUserDataString(const WCHAR* key, WCHAR* data) {
	return InterfaceSugar::Instance().SetCandidateUserDataString(key, data);
}

extern "C" INT_PTR SetCandidateUserDataNumeric(const WCHAR* key, INT_PTR data) {
	return InterfaceSugar::Instance().SetCandidateUserDataNumeric(key,data);
}

extern "C" INT_PTR AppendChildCandidate(WCHAR*candidate, WCHAR*description) {
	return InterfaceSugar::Instance().AppendChildCandidate(candidate, description);
}

/////////////////////////////////////////////////////////////////////////////
//Kind制作者が利用する関数
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR GetSelectionCount() {
	return Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateCount();
}

extern "C" WCHAR* GetSelectionText(INT_PTR selected_index) {
	auto index=Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetText(index));
}

extern "C" WCHAR* GetSelectionActionDirectoryName(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetActionDirectoryName(index));
}

extern "C" WCHAR* GetSelectionActionFileName(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetActionFileName(index));
}

extern "C" INT_PTR GetSelectionActionColumn(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates().GetActionColumn(index);
}

extern "C" INT_PTR GetSelectionActionLine(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates().GetActionLine(index);
}

extern "C" WCHAR* GetSelectionActionCommand(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetActionCommand(index));
}

extern "C" INT_PTR GetSelectionActionProcessId(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates().GetActionProcessId(index);
}

extern "C" INT_PTR GetSelectionActionWindowHandle(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates().GetActionWindowHandle(index);
}

extern "C" WCHAR* GetSelectionSourceName(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetSourceName(index));
}

extern "C" WCHAR* GetSelectionDescription(INT_PTR selected_index) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetDescription(index));
}

extern "C" WCHAR* GetSelectionUserDataString(INT_PTR selected_index, WCHAR*key_name, WCHAR*default_value) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetUserData(index,key_name, default_value));
}

extern "C" INT_PTR GetSelectionUserDataNumeric(INT_PTR selected_index, WCHAR*key_name, INT_PTR default_value) {
	auto index = Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
	return Unity::Instance().lock()->QueryCandidates().GetUserData(index, key_name, default_value);
}

extern "C" INT_PTR StartCandidate(WCHAR*source_name, WCHAR*arg) {	
	return Unity::Instance().lock()->QueryStaticStatus().GetIsStart().Set(source_name,arg);
}

//コールバック
extern "C" INT_PTR AppendCallBack_ProcessFinish(WCHAR*macro_filename, WCHAR*label_name, WCHAR* args) {
	return Unity::Instance().lock()->QueryStaticStatus().m_process_finish_callbackes.Append(macro_filename, label_name, args);
}

extern "C" INT_PTR GetCallBack_ProcessFinish_Size() {
	return Unity::Instance().lock()->QueryStaticStatus().m_process_finish_callbackes.Size();
}

extern"C" WCHAR* GetCallBack_ProcessFinish_MacroFilename(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().m_process_finish_callbackes.GetMacroFileName(index));
}

extern"C" WCHAR* GetCallBack_ProcessFinish_LabelName(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().m_process_finish_callbackes.GetLabelName(index));
}

extern"C" WCHAR* GetCallBack_ProcessFinish_Args(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().m_process_finish_callbackes.GetArgs(index));
}


/////////////////////////////////////////////////////////////////////////////
//Unity
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR PushContext(INT_PTR if_exist_context_then_delete) {
	return Unity::PushContext(if_exist_context_then_delete);
}

extern "C" INT_PTR PopContext(INT_PTR if_exist_context_then_delete) {
	return Unity::PopContext(if_exist_context_then_delete);
}

extern "C" INT_PTR GetCurrentContext() {
	return Unity::GetCurrentInstanceIndex();
}

extern "C" INT_PTR SerializeCurrentContext(WCHAR* out_filename) {
	return Unity::Instance().lock()->SerializeStaticContext(out_filename);
}

extern "C" INT_PTR DeSerializeToCurrentContext(WCHAR* input_filename) {
	return Unity::Instance().lock()->DeSerializeToStaticContext(input_filename);
}

extern "C" INT_PTR SerializeStaticStatusContext(WCHAR* out_filename) {
	return Unity::Instance().lock()->SerializeStaticStatusContext(out_filename);
}

extern "C" INT_PTR DeSerializeToStaticStatusContext(WCHAR* input_filename) {
	return Unity::Instance().lock()->DeSerializeToStaticStatusContext(input_filename);
}

extern "C" INT_PTR SetUserDataString(WCHAR* key, WCHAR* data) {
	return Unity::Instance().lock()->QueryUserData().SetUserData(key,data);
}

extern "C" WCHAR* GetUserDataString(WCHAR* key, const WCHAR*	default_data) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryUserData().GetUserData(key, default_data));
}

extern "C" INT_PTR HasChangedCandidates() {
	return Unity::Instance().lock()->HasChangedCandidates();
}

extern "C" INT_PTR  ClearChangedCandidatesAndReturnPrevStatus() {
	return Unity::Instance().lock()->ClearChangedCandidatesAndReturnPrevStatus();
}


extern "C" INT_PTR StatusUpdate(const WCHAR*kind_name, const WCHAR*action_name, INT_PTR context_index) {
	return Unity::Instance().lock()->StatusUpdate(kind_name,action_name,context_index);
}

extern "C" INT_PTR RecurringTask() {
	Unity::Instance().lock()->QueryRecurringTask().Update();
	return 1;
}
extern "C" INT_PTR AutoPreviewRegist(const WCHAR* filename) {
	Unity::Instance().lock()->AutoPreviewRegist(filename);
	return 1;
}
extern "C" WCHAR* GetInteractiveSourceNames(const WCHAR* separator){
	return const_cast<WCHAR*>(Unity::Instance().lock()->GetInteractiveSourceNames(separator));
}
/*
extern "C" INT_PTR GetSourceNameForCandidatesNum() {
	return Unity::Instance().lock()->QueryCandidates().GetSourceNameForCandidatesNum();
}
extern "C" WCHAR* GetSourceNameForCandidates(INT_PTR index) {
    return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetSourceNameForCandidates(index));
}*/

/////////////////////////////////////////////////////////////////////////////
//絞り込み検索
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR RefineSearchChangeMarked(INT_PTR hidemaru_line_no, INT_PTR is_selected) {	
	return Unity::Instance().lock()->QueryRefineSearch().ChangeMarked(hidemaru_line_no, is_selected);
}

extern "C" INT_PTR RefineSearchGetMarkedCount() {
	return Unity::Instance().lock()->QueryRefineSearch().GetMarkedCount();
}

extern "C" INT_PTR RefineSearchConvertSelectedIndexToHidemaruLineno(INT_PTR index) {
	return Unity::Instance().lock()->QueryRefineSearch().ConvertSelectedIndexToHidemaruLineno(index);
}

extern "C" INT_PTR RefineSearchConvertHidemaruLinenNoToCandidateIndex(INT_PTR hidemaru_line_no) {
	return Unity::Instance().lock()->QueryRefineSearch().ConvertHidemaruLinenNoToCandidateIndex(hidemaru_line_no);
}

extern "C" INT_PTR RefineSearchSetHidemaruLineno(INT_PTR hidemaru_line_no) {
	Unity::Instance().lock()->QueryRefineSearch().SetHidemaruLineno(hidemaru_line_no);
	return true;
}

extern "C" INT_PTR RefineSearchDo(WCHAR* search_words) {
	return Unity::Instance().lock()->QueryRefineSearch().Do(search_words);
}

extern "C" WCHAR* RefineSearchGetResult() {
	return Unity::Instance().lock()->QueryRefineSearch().GetResult();
}

extern "C" INT_PTR RefineSearchGetFirstSelectionCandidateIndex() {
	return Unity::Instance().lock()->QueryRefineSearch().GetFirstSelectionCandidateIndex();
}

extern "C" INT_PTR	RefineSearchGetSelectionCandidateCount() {
	return GetSelectionCount();
}

extern "C" INT_PTR	RefineSearchGetSelectionCandidateIndex(INT_PTR selected_index) {
	return Unity::Instance().lock()->QueryRefineSearch().GetSelectionCandidateIndex(selected_index);
}

extern "C" INT_PTR	RefineSearchMoveHidemaruCursorLineNo(INT_PTR current_line_no, INT_PTR delta) {
	return Unity::Instance().lock()->QueryRefineSearch().MoveHidemaruCursorLineNo(current_line_no, delta);
}

/////////////////////////////////////////////////////////////////////////////
//source
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR SourcesLoadAll(const WCHAR* directory) {
	return Unity::Instance().lock()->QuerySources().LoadSourceAll(directory);
}

extern "C" INT_PTR SetSourceRootPath(const WCHAR* root_path) {
	Unity::Instance().lock()->QuerySources().SetRootPath(root_path);
	return true;
}

extern "C" WCHAR* GetSourceRootPath() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QuerySources().GetRootPath().c_str());
}
/*
extern "C" WCHAR* SourcesCreate(WCHAR* source_ini){
	gs_path_pointers.m_source_created.assign(Unity::Instance().lock()->QuerySources().Create(source_ini));
    return const_cast<WCHAR*>(gs_path_pointers.m_source_created.c_str());
}*/

extern "C" WCHAR* SourcesCreateFromIniImage(const WCHAR* source_filename, const WCHAR* ini_image){
	auto& source = Unity::Instance().lock()->QuerySources();

	WCHAR fname[_MAX_FNAME];
	errno_t err = _wsplitpath_s(
		source_filename,
		nullptr, 0,
		nullptr, 0,
		fname, _countof(fname),
		nullptr, 0
	);
	if (err != 0) {
		return const_cast<WCHAR*>(L"");
	}
	auto source_name = source.Create(fname,ini_image);
	if (source_name.compare(L"") == 0) {
		return const_cast<WCHAR*>(L"");
	}
	if (source.AppendFileNameAndSourceName(source_filename, source_name.c_str())==false) {
		return const_cast < WCHAR*>(L"");
	}
    gs_path_pointers.m_source_created.assign(source_name);
    return const_cast<WCHAR*>(gs_path_pointers.m_source_created.c_str());
}

extern "C" WCHAR* SourcesCreateFromIniFile(const WCHAR* source_filename, const WCHAR* ini_filename){
	auto&& unity = Unity::Instance().lock();
	std::wstring ini_image;
	if (unity->QueryFile().ReadFile(ini_image, ini_filename) == false) {
        return const_cast<WCHAR*>(L"");
	}
	return SourcesCreateFromIniImage(source_filename, ini_image.c_str());
}

extern "C" WCHAR* SourcesGetCandidateType(WCHAR*souce_name) {
	auto*source = Unity::Instance().lock()->QuerySources().FindSource(souce_name);	
	return const_cast<WCHAR*>(source->m_candidate_type.c_str());
}

extern "C" INT_PTR SourcesGetInteractive(WCHAR*souce_name) {
	auto*source = Unity::Instance().lock()->QuerySources().FindSource(souce_name);	
	return source->m_is_interactive;
}

extern "C" WCHAR* SourcesGetDefaultKind(WCHAR*souce_name) {
	auto*source = Unity::Instance().lock()->QuerySources().FindSource(souce_name);	
	return const_cast<WCHAR*>(source->m_default_kind.c_str());
}

extern "C" INT_PTR SourcesAppendFileNameAndSourceName(const WCHAR*file_name, const WCHAR*source_name){
	return Unity::Instance().lock()->QuerySources().AppendFileNameAndSourceName(file_name, source_name);
}
extern "C" INT_PTR SourcesAppendFileNameAndSourceNameIfNotExist(const WCHAR*source_name){
	auto& sources = Unity::Instance().lock()->QuerySources();
	if (sources.FindSource(source_name) == nullptr) {
		//ソースが存在しない場合は追加する

		std::wstring source_filenme;
		source_filenme.append(sources.GetRootPath());
        source_filenme.append(source_name);

		std::wstring ini_filenme;
		ini_filenme.append(sources.GetRootPath());
		ini_filenme.append(source_name);
        ini_filenme.append(L".ini");
		auto result = SourcesCreateFromIniFile(source_filenme.c_str(), ini_filenme.c_str());
		if (wcscmp(result, L"") == 0) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}

extern "C" INT_PTR SourcesExistFileName(const WCHAR*file_name) {
	return Unity::Instance().lock()->QuerySources().ExistFileName(file_name);
}

extern "C" WCHAR* SourcesFileNameToSourceName(const WCHAR*file_name) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QuerySources().FileNameToSourceName(file_name));
}

extern "C" WCHAR* SourcesSourceNameToFileName(const WCHAR*source_name) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QuerySources().SourceNameToFileName(source_name));
}

/////////////////////////////////////////////////////////////////////////////
//候補
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR CandidatesAppendHeader(WCHAR*source_name, WCHAR*header, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates().AppendCandidateHeader(source_name, header, description);
}

extern "C" INT_PTR CandidatesAppend(WCHAR*source_name, WCHAR*candidate, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates().AppendCandidate(source_name, candidate, description);
}

extern "C" INT_PTR CandidatesAppendChild(INT_PTR candidate_index, WCHAR*candidate, WCHAR*description) {
	return Unity::Instance().lock()->QueryCandidates().AppendChildCandidate(candidate_index, candidate, description);
}

extern "C" INT_PTR CandidatesSetUserDataString(INT_PTR index, const WCHAR* key, const WCHAR*data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(index, key,data);
}

extern "C" INT_PTR CandidatesSetUserDataNumeric(INT_PTR index, const WCHAR* key, INT_PTR data) {
	return Unity::Instance().lock()->QueryCandidates().SetUserData(index, key, data);
}

extern "C" const WCHAR*	CandidatesGetUserDataString(INT_PTR index, const WCHAR* key, const WCHAR*	default_data) {
	return Unity::Instance().lock()->QueryCandidates().GetUserData(index, key, default_data);
}

extern "C" INT_PTR CandidatesGetUserDataNumeric(INT_PTR index, const WCHAR* key, INT_PTR default_data) {
	return Unity::Instance().lock()->QueryCandidates().GetUserData(index,key,default_data);
}

extern "C" WCHAR* CandidatesGetSourceName(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetSourceName(index));
}

extern "C" WCHAR* CandidatesGetText(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryCandidates().GetText(index));
}

extern "C" INT_PTR CandidatesClearWithSourceName(const WCHAR*source_name) {
	return Unity::Instance().lock()->CandidatesClearWithSourceName(source_name);
}


/////////////////////////////////////////////////////////////////////////////
//kind
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR KindsLoadAll(const WCHAR* directory) {
	return Unity::Instance().lock()->QueryKinds().LoadKindAll(directory);
}

extern "C" INT_PTR KindsGenerateCandidates(INT_PTR instance_index) {
	return Unity::Instance().lock()->QueryKinds().GenerateKindCandidates(instance_index);
}

extern "C" WCHAR* KindsGetDefaultActionLabelName(WCHAR* kind_name) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetDefaultActionLabelName(kind_name));
}

extern "C" INT_PTR	KindsFindKindIndex(const WCHAR* kind_name) {
	return Unity::Instance().lock()->QueryKinds().FindKindIndex(kind_name);
}

extern "C" INT_PTR	KindsFindActionIndex(INT_PTR kind_index, const WCHAR* action_name) {
	return Unity::Instance().lock()->QueryKinds().FindActionIndex(kind_index, action_name);
}

//
// Kind
// 
extern "C" WCHAR* KindGetKindName(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetKindName(kind_index));
}

extern "C" WCHAR* KindGetKindDescription(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetKindDescription(kind_index));
}

extern "C" WCHAR* KindGetDefaultAction(INT_PTR kind_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetDefaultAction(kind_index));
}

//
// Action
// 
extern "C" WCHAR*	KindsGetActionName(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetActionName(kind_index, action_index));
}

extern "C" WCHAR*	KindsGetActionLabelName(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetActionLabelName(kind_index, action_index));
}

extern "C" WCHAR*	KindsGetActionDescription(INT_PTR kind_index, INT_PTR action_index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryKinds().GetActionDescription(kind_index, action_index));
}

extern "C" INT_PTR	KindsIsActionQuit(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds().IsActionQuit(kind_index, action_index);
}

extern "C" INT_PTR	KindsIsActionMultiSelectable(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds().IsActionMultiSelectable(kind_index, action_index);
}

extern "C" INT_PTR	KindsIsActionStart(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds().IsActionStart(kind_index, action_index);
}

extern "C" INT_PTR	KindsIsEdit(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds().IsEdit(kind_index, action_index);
}

extern "C" INT_PTR	KindsIsRegetCandidates(INT_PTR kind_index, INT_PTR action_index) {
	return Unity::Instance().lock()->QueryKinds().IsRegetCandidates(kind_index, action_index);
}


/////////////////////////////////////////////////////////////////////////////
//Inheritance
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR InheriatnceGenerateDefaultAction(const WCHAR*source_name) {
	auto&& unity = Unity::Instance().lock();
	auto& sources = unity->QuerySources();
	auto* source = sources.FindSource(source_name);
	if (source==nullptr) {
        //ソースが存在しない場合は追加する
		std::wstring ini_filename;
		ini_filename.assign(sources.GetRootPath());
        ini_filename.append(source_name);
        ini_filename.append(L".ini");

		std::wstring source_filename;
		source_filename.assign(sources.GetRootPath());
		source_filename.append(source_name);
		//source_filename.append(L".mac");
        auto result = SourcesCreateFromIniFile(source_filename.c_str(), ini_filename.c_str());
		if (wcscmp(result, L"")==0) {
			return false;
		}
		else {
			//pass
		}
	}

	return unity->QueryInheritance().GenerateDefaultAction(source_name);
}

extern "C" WCHAR*	InheriatnceGetDefaultActionKind() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryInheritance().GetDefaultAction().m_kind_name.c_str());
}

extern "C" WCHAR*	InheriatnceGetDefaultActionLabel() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryInheritance().GetDefaultAction().m_label_name.c_str());
}

extern "C" WCHAR* InheritanceFindActionKind(const WCHAR* action_name) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryInheritance().FindActionKind(action_name));
}


/////////////////////////////////////////////////////////////////////////////
//StaticStatus
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR StaticStatusInitialize(INT_PTR target_hidemaru, INT_PTR search_hidemaru_handle, const WCHAR* working_directory, const WCHAR*root_macro_directory) {
	Unity::Instance().lock()->QueryStaticStatus().Initialize(target_hidemaru, search_hidemaru_handle, working_directory, root_macro_directory);
	return true;
}

extern "C" INT_PTR StaticStatusSetIsStart(INT_PTR is_start) {
	return Unity::Instance().lock()->QueryStaticStatus().GetIsStart().SetEnable(is_start);
}

extern "C" INT_PTR StaticStatusClearIsStart() {
	return Unity::Instance().lock()->QueryStaticStatus().GetIsStart().Clear();
}

extern "C" INT_PTR StaticStatusEnableIsStart() {
	return Unity::Instance().lock()->QueryStaticStatus().GetIsStart().Enable();
}

extern "C" WCHAR* StaticStatusGetIsStartSourceName() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().GetIsStart().m_source_name.c_str());
}

extern "C" WCHAR* StaticStatusGetIsStartArg() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().GetIsStart().m_arg.c_str());
}

extern"C" INT_PTR StaticStatusSetIsQuit(INT_PTR is_quit) {
	Unity::Instance().lock()->QueryStaticStatus().m_is_quit = is_quit;
	return true;
}

extern"C" INT_PTR StaticStatusGetIsQuit() {
	return Unity::Instance().lock()->QueryStaticStatus().m_is_quit;
}

extern"C" INT_PTR StaticStatusSetTargetHidemaruHandle(INT_PTR hidemaru_handle) {
	Unity::Instance().lock()->QueryStaticStatus().m_target_hidemaruhandle=hidemaru_handle;
	return true;
}

extern"C" INT_PTR StaticStatusGetTargetHidemaruHandle() {
	return Unity::Instance().lock()->QueryStaticStatus().m_target_hidemaruhandle;
}

extern"C" INT_PTR StaticStatusSetSearchHidemaruHandle(INT_PTR hidemaru_handle) {
	Unity::Instance().lock()->QueryStaticStatus().m_search_hidemaru_handle = hidemaru_handle;
	return true;
}

extern"C" INT_PTR StaticStatusGetSearchHidemaruHandle() {
	return Unity::Instance().lock()->QueryStaticStatus().m_search_hidemaru_handle;
}

extern"C" INT_PTR StaticStatusSetCurrentWorkingDirectory(WCHAR* current_working_directory) {
	Unity::Instance().lock()->QueryStaticStatus().m_current_working_directory.assign(current_working_directory);
	return true;
}

extern"C" WCHAR* StaticStatusGetCurrentWorkingDirectory() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().m_current_working_directory.c_str());
}

extern"C" INT_PTR StaticStatusSetRootMacroDirectory(WCHAR* root_macro_directory) {
	Unity::Instance().lock()->QueryStaticStatus().m_root_macro_directory.assign(root_macro_directory);
	return true;
}

extern"C" WCHAR* StaticStatusGetRootMacroDirectory() {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().m_root_macro_directory.c_str());
}

/////////////////////////////////////////////////////////////////////////////
//ContextStatus
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR StaticStatusGetNumberOfSourceNamesForReacquisitionCandidates() {
	return Unity::Instance().lock()->QueryStaticStatus().GetNumberOfSourceNamesForReacquisitionCandidates();
}

extern "C" WCHAR* StaticStatusGetSourceNameForReacquisitionCandidates(INT_PTR index) {
	return const_cast<WCHAR*>(Unity::Instance().lock()->QueryStaticStatus().GetSourceNameForReacquisitionCandidates(index));
}

extern "C" INT_PTR StaticStatusClearSourceNamesForReacquisitionCandidates() {
	Unity::Instance().lock()->QueryStaticStatus().ClearSourceNamesForReacquisitionCandidates();
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
//file
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR WriteToFile(const WCHAR* filename, const WCHAR* string) {
	return Unity::Instance().lock()->QueryFile().WriteToFile(filename, string);
}

extern "C" WCHAR* PathSplit(const WCHAR* path, const WCHAR*separator) {
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];

	errno_t err = _wsplitpath_s(
		path,
		drive, _countof(drive),
		dir, _countof(dir),
		fname, _countof(fname),
        ext, _countof(ext)
	);
	if (err!=0) {
        return const_cast<WCHAR*>(L"");
	}
    std::wstring result;
    result.append(drive);
    result.append(separator);
	result.append(dir);
	result.append(separator);
	result.append(fname);
	result.append(separator);
	result.append(ext);
    gs_path_pointers.m_path_split.swap(result);
    return const_cast<WCHAR*>(gs_path_pointers.m_path_split.c_str());
}

extern "C" WCHAR* PathBaseName(const WCHAR* path) {
	WCHAR fname[_MAX_FNAME];
	errno_t err = _wsplitpath_s(
		path,
		nullptr, 0,
		nullptr, 0,
		fname, _countof(fname),
		nullptr, 0
	);
	if (err != 0) {
		return const_cast<WCHAR*>(L"");
	}
	gs_path_pointers.m_basename.assign(fname);
    return const_cast<WCHAR*>(gs_path_pointers.m_basename.c_str());
}

extern "C" INT_PTR FileRegistAfterDeleteFile(const WCHAR* filename) {
	return Unity::Instance().lock()->QueryFile().RegistAfterDelete(filename);
}

extern "C" INT_PTR FileUnRegistAfterDeleteFile(const WCHAR* filename) {
	return Unity::Instance().lock()->QueryFile().UnRegistAfterDelete(filename);
}

extern "C" INT_PTR DllDetachFunc_After_Hm866( INT_PTR n  ) {	
	DebugLog(_T("DllDetachFunc_After_Hm866 (%d)"), n); 
	Unity::Destroy();
    gs_path_pointers.Destroy();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//Log
/////////////////////////////////////////////////////////////////////////////
extern "C" INT_PTR LogEnable(INT_PTR enable) {
	DebugLogEnable(enable);
	return true;
}
