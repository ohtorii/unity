#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };

std::array<std::shared_ptr<Unity>, UNITY_MAX_CONTEXT_NUM>	Unity::m_instances{nullptr, };
size_t					Unity::m_current_instance_index = 0;
Sources					Unity::m_sources;
Kinds					Unity::m_kinds;
Status					Unity::m_status;



///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
std::weak_ptr<Unity> Unity::Instance(size_t index) {
	if (index == UNITY_USE_URRENT_INSTANCE) {
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
			//archive(*(Instance().lock()));
			archive(
				Unity::m_instances,
				Unity::m_current_instance_index,
				Unity::m_sources,
				Unity::m_kinds,
				Unity::m_status,
				InterfaceSugar::m_instance);
		}
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
				Unity::m_status, 
				InterfaceSugar::m_instance);
		}
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

 bool Unity::SerializeStatusContext(const WCHAR*out_filename) {
	 try {
		 std::ofstream os(out_filename, std::ios::binary);
		 if (!os) {
			 return false;
		 }

		 {
			 cereal::BinaryOutputArchive archive(os);
			 //archive(*(Instance().lock()));
			 archive(Unity::m_status);
		 }
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

 bool Unity::DeSerializeToStatusContext(const WCHAR*input_filename) {
	 try {
		 std::ifstream is(input_filename, std::ios::binary);
		 if (!is) {
			 return false;
		 }
		 {
			 cereal::BinaryInputArchive	archive(is);
			 archive(Unity::m_status);
		 }
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

void Unity::Destroy() {
	for (size_t i = 0; i<m_instances.size() ; ++i) {
		m_instances.at(i).reset();
	}
	m_kinds.Clear();
	m_current_instance_index = 0;
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

Status& Unity::QueryStatus() {
	return m_status;
}

ASyncFiles&		Unity::QueryASyncFiles() {
	return m_async_files;
}

Unity::Unity() : m_refine_search(this), m_inheritance(this), m_async_files(this){
}

Unity::~Unity(){
}
