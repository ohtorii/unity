#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
std::array<Unity*, 4>	Unity::m_instances{ {nullptr,nullptr,nullptr,nullptr} };
size_t					Unity::m_current_instance_index = 0;
Kinds					Unity::m_kinds;


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
Unity* Unity::Instance(){
	if(m_instances.at(m_current_instance_index)==nullptr){
		m_instances.at(m_current_instance_index)=new Unity;
	}
	return m_instances.at(m_current_instance_index);
}

bool Unity::Push() {
	if (m_instances.size() <= (m_current_instance_index+1)) {
		return false;
	}
	++m_current_instance_index;
	m_instances.at(m_current_instance_index) = new Unity;
	return true;
}

bool Unity::Pop() {
	if (m_current_instance_index == 0) {
		//これ以上popできない
		return false;
	}
	delete m_instances.at(m_current_instance_index);
	m_instances[m_current_instance_index] = nullptr;
	--m_current_instance_index;
	return true;
}

size_t Unity::GetCurrentInstanceIndex() {
	return m_current_instance_index;
}

void Unity::Destroy() {
	for (size_t i = 0; i<m_instances.size() ; ++i) {
		delete m_instances.at(i);
		m_instances.at(i) = nullptr;
	}
	m_current_instance_index = 0;
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


Unity::Unity(){
	
}

Unity::~Unity(){
	
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


