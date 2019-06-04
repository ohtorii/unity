﻿#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };

std::array<std::shared_ptr<Unity>, 4>	Unity::m_instances{ {nullptr,nullptr,nullptr,nullptr} };
size_t					Unity::m_current_instance_index = 0;
Kinds					Unity::m_kinds;


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
		OutputDebugString(_T("Unity::PopContext -> m_current_instance_index == 0"));
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
			archive(Unity::m_instances,Unity::m_current_instance_index);
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
			//archive(*(Instance().lock()));
			archive(Unity::m_instances, Unity::m_current_instance_index);
		}
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

void Unity::Destroy() {
	DebugLog(_T("Unity::Destroy() Start"));
	for (size_t i = 0; i<m_instances.size() ; ++i) {
		m_instances.at(i).reset();
	}
	m_kinds.Clear();
	m_current_instance_index = 0;
	DebugLog(_T("Unity::Destroy() Finish"));
}

Sources* Unity::QuerySources(){
	return &m_sources;
}

File*	Unity::QueryFile(){
	return &m_file;
}

Candidates*	Unity::QueryCandidates() {
	return &m_candidates;
}

RefineSearch*	Unity::QueryRefineSearch() {
	return &m_refine_search;
}

Kinds*			Unity::QueryKinds() {
	return &m_kinds;
}

Inheritance*	Unity::QueryInheritance() {
	return &m_inheritance;
}


Unity::Unity() : m_refine_search(this), m_inheritance(this){
	DebugLog(_T("Unity::Unity() %p"),this);
}

Unity::~Unity(){
	DebugLog(_T("Unity::~Unity() %p"),this);
}


#if 0
INT_PTR Unity::SetCandidateList(WCHAR* source_filename,WCHAR*source_name,WCHAR*source_description){
	return LoadTextFile(source_filename);
}

bool Unity::LoadTextFile(const WCHAR*filename) {
	//OutputDebugString(L"==== Loadtext start ====");
	//OutputDebugString(filename);
	m_input.clear();

	FILE *fp = 0;
	const errno_t error = _wfopen_s(&fp, filename, L"rb");
	if (error != 0) {
		//OutputDebugString(L"@1");
		return false;
	}
	if (fp == 0) {
		return false;
	}

	bool result = false;
	std::vector<WCHAR> line;
	line.resize(64 * 1024);
	try {
		if (m_input.capacity() == 0) {
			m_input.reserve(1024);
		}
		while (true) {
			if (fgetws(&line[0], line.size(), fp) == 0) {
				break;
			}
			//OutputDebugString(line);
			m_input.emplace_back(&line[0]);
		}
		//OutputDebugString(L"@2");
		result = true;
	}
	catch (std::exception) {
		//pass
		//OutputDebugString(L"@3");
		result = false;
	}

	fclose(fp);
	//std::sort(gs_text.begin(), gs_text.end());
	//OutputDebugString(L"==== Loadtext finish ====");
	//OutputDebugString(L"@4");
	return result;
}
#endif


