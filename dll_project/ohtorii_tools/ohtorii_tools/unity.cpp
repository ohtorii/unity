#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };

std::array<std::shared_ptr<Unity>, UNITY_MAX_CONTEXT_NUM>	Unity::m_instances{nullptr, };
size_t					Unity::m_current_instance_index = 0;
Sources					Unity::m_sources;
Kinds					Unity::m_kinds;
StaticStatus			Unity::m_static_status;
HidemaruFunctions		Unity::m_hidemaru_functions;


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
std::weak_ptr<Unity> Unity::Instance(size_t index) {
	if (index == UNITY_USE_CURRENT_INSTANCE) {
		if (m_instances.at(m_current_instance_index) == nullptr) {
			m_instances.at(m_current_instance_index) = std::make_shared<Unity>();
		}
		return m_instances.at(m_current_instance_index);
	}
	return m_instances.at(index);
}

bool Unity::PushContext(bool exist_context_then_delete) {
	DebugLog(_T("Unity::PushContext(%d)"), m_current_instance_index);
	if (m_instances.size() <= (m_current_instance_index+1)) {
		DebugLog(_T("Unity::PopContext -> m_instances.size() <= (m_current_instance_index+1)"));
		return false;
	}
	++m_current_instance_index;

	if (m_instances.at(m_current_instance_index)) {
		if (exist_context_then_delete) {
			m_instances[m_current_instance_index].reset();
			m_instances[m_current_instance_index] = std::make_shared<Unity>();
		}else {
			//pass
		}
	}else {
		m_instances[m_current_instance_index] = std::make_shared<Unity>();
	}
	return true;
}

bool Unity::PopContext(bool exist_context_then_delete) {
	DebugLog(_T("Unity::PopContext(%d)"), m_current_instance_index);
	if (m_current_instance_index == 0) {
		//これ以上popできない
		DebugLog(_T("Unity::PopContext -> m_current_instance_index == 0"));
		return false;
	}
	if (exist_context_then_delete) {
		m_instances.at(m_current_instance_index).reset();
		m_instances[m_current_instance_index] = nullptr;
	}
	--m_current_instance_index;
	return true;
}

size_t Unity::GetCurrentInstanceIndex() {
	return m_current_instance_index;
}

bool Unity::SerializeCurrentContext(const WCHAR*out_filename) {
	try {
		std::ofstream os(out_filename, std::ios::binary);
		if (!os) {
			return false;
		}
		
		{
			cereal::BinaryOutputArchive archive(os);
			archive(
				Unity::m_instances,
				Unity::m_current_instance_index,
				Unity::m_sources,
				Unity::m_kinds,
				Unity::m_static_status,
				InterfaceSugar::m_instance);
		}
		os.close();
		return true;
	}catch (std::exception) {
		//pass
	}
	return false;
}

bool Unity::DeSerializeToCurrentContext(const WCHAR*input_filename) {
	try {
		std::ifstream is(input_filename, std::ios::binary);
		if (!is) {
			return false;
		}
		{
			cereal::BinaryInputArchive	archive(is);
			archive(
				Unity::m_instances, 
				Unity::m_current_instance_index, 
				Unity::m_sources,
				Unity::m_kinds, 
				Unity::m_static_status, 
				InterfaceSugar::m_instance);
		}
		is.close();
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

 bool Unity::SerializeStaticStatusContext(const WCHAR*out_filename) {
	 try {
		 std::ofstream os(out_filename, std::ios::binary);
		 if (!os) {
			 return false;
		 }

		 {
			 cereal::BinaryOutputArchive archive(os);
			 archive(Unity::m_static_status/*, Unity::Instance().lock()->m_candidates*/);
		 }
		 os.close();
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

 bool Unity::DeSerializeToStaticStatusContext(const WCHAR*input_filename) {
	 try {
		 std::ifstream is(input_filename, std::ios::binary);
		 if (!is) {
			 return false;
		 }
		 {
			 cereal::BinaryInputArchive	archive(is);
			 archive(Unity::m_static_status/*, Unity::Instance().lock()->m_candidates*/);
		 }
		 is.close();
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

void Unity::Destroy() {
	for (size_t i = 0; i<m_instances.size() ; ++i) {
		auto&item = m_instances.at(i);
		if (item==nullptr) {
			continue;
		}
		item->QueryASyncFiles().Destroy();
		item.reset();
	}
	m_kinds.Clear();
	m_current_instance_index = 0;

	//
	//memo: ASyncFileでファイルを閉じた後で終了処理を行う。
	//そうしないと、ASyncFileがファイルを開いているためファイルの削除が出来ない。
	//
	//今のところ削除するファイルは1個でパフォーマンスに大きな影響は無いため対策は行わないことにした。
	//
	File::StartDestroy();
	File::JoinDestroy();
}

Sources& Unity::QuerySources(){
	return m_sources;
}

File&	Unity::QueryFile(){
	return m_file;
}

Candidates&	Unity::QueryCandidates() {
	return m_candidates;
}

RefineSearch&	Unity::QueryRefineSearch() {
	return m_refine_search;
}

Kinds&			Unity::QueryKinds() {
	return m_kinds;
}

Inheritance&	Unity::QueryInheritance() {
	return m_inheritance;
}

UserData&		Unity::QueryUserData() {
	return m_user_data;
}

ContextStatus&	Unity::QueryContextStatus() {
	return m_context_status;
}

StaticStatus& Unity::QueryStaticStatus() {
	return m_static_status;
}

ASyncFiles&		Unity::QueryASyncFiles() {
	return m_async_files;
}

RecurringTask&	Unity::QueryRecurringTask() {
	return m_recurring_task;
}

HidemaruFunctions& Unity::QueryHidemaruFunctions() {
	return m_hidemaru_functions;
}

void Unity::ChangeCandidates() {
	m_changed_candidates = true;
}

bool Unity::HasChangedCandidates() {
	return m_changed_candidates;
}

bool Unity::ClearChangedCandidatesAndReturnPrevStatus() {
	auto prev = m_changed_candidates;
	m_changed_candidates = false;
	return prev;
}


bool Unity::StatusUpdate(const WCHAR*kind_name, const WCHAR*action_name, INT_PTR context_index) {
	auto kind_index = m_kinds.FindKindIndex(kind_name);
	if (kind_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}

	auto action_index = m_kinds.FindActionIndex(kind_index, action_name);
	if (action_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	m_static_status.UpdateStatus(kind_index, action_index);
	m_context_status.UpdateStatus(kind_index, action_index, context_index);
	return true;
}

Unity::Unity() : 
	m_refine_search(this), 
	m_inheritance(this),
	m_async_files(this)
{
	m_changed_candidates = false;
}

Unity::~Unity(){

}

bool Unity::AutoPreviewRegist(const WCHAR*filename) {
	return m_recurring_task.Register<AutoPreview>(filename);
}
