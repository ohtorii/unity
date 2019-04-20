#include"stdafx.h"



///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static bool StringToBool(const WCHAR* str) {
	if (_wcsicmp(str, _T("false")) == 0) {
		return false;
	}
	if (_wcsicmp(str, _T("0")) == 0) {
		return false;
	}
	return true;
}

static void Tokenize(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters = _T(" "))
{
	tokens.clear();
	// Skip delimiters at beginning.
	std::wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::wstring::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::wstring::npos != pos || std::wstring::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

static void GatherActionSections(std::vector<std::wstring> &dst, const WCHAR* filename) {
	WCHAR buf[2 * 1024];
	//[action.*]セクションのパース
	static const std::wstring	prefix(_T("action."));
	const		size_t			prefix_size = prefix.size();
	
	GetPrivateProfileSectionNames(buf, _countof(buf), filename);
	
	size_t i = 0;
	while (((i + 1) < _countof(buf)) && (buf[i] != 0) && (buf[i + 1] != 0)) {
		WCHAR* top = &buf[i];
		//OutputDebugString(top);
		if (std::equal(prefix.begin(), prefix.end(), top)) {
			dst.push_back(top);
			//OutputDebugString(top);
		}
		i += wcslen(top) + 1;//+1で'\0'を読み飛ばす
	}
}

static void ParseActionSection(Action &dst, const WCHAR*section_name, const WCHAR* filename) {
	/*アクションのセクションをパースする
	*/
	WCHAR buf[2*1024];

	dst.m_name.assign(section_name);

	GetPrivateProfileString(section_name, _T("function"), _T(""), buf, _countof(buf), filename);
	dst.m_function.assign(buf);

	GetPrivateProfileString(section_name, _T("description"), _T(""), buf, _countof(buf), filename);
	dst.m_description.assign(buf);

	GetPrivateProfileString(section_name, _T("is_quit"), _T(""), buf, _countof(buf), filename);
	dst.m_is_quit = StringToBool(buf);
	
	GetPrivateProfileString(section_name, _T("is_multi_selectable"), _T(""), buf, _countof(buf), filename);
	dst.m_is_multi_selectable = StringToBool(buf);
}

static bool CheckAppendable(bool candidate_is_multi_select, bool action_is_multi_selectable) {
	if (candidate_is_multi_select) {
		if (action_is_multi_selectable) {
			return true;
		}
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
//	class Action
///////////////////////////////////////////////////////////////////////////////
Action::Action() {
	m_is_quit = false;
	m_is_multi_selectable = false;
}


///////////////////////////////////////////////////////////////////////////////
//	class Kind
///////////////////////////////////////////////////////////////////////////////
Kind::Kind() {

}

Kind::Kind(const std::wstring		&name,
	const std::wstring				&description,
	const std::wstring				&default_action,
	const std::vector<std::wstring>	&inheritance):
	m_name(name),
	m_description(description),
	m_default_action(default_action),
	m_inheritance(inheritance)
{

}



///////////////////////////////////////////////////////////////////////////////
//	class Kinds
///////////////////////////////////////////////////////////////////////////////
void Kinds::Clear() {
	m_kinds.clear();
}

WCHAR* Kinds::Create(const WCHAR* kind_ini) {
	static std::wstring	name;	//staticについて: 秀丸エディタへ文字列を返すため静的なメモリ領域とする
	Kind dst;

//	std::wstring	description;
	//std::wstring	default_action;
	//std::vector<std::wstring>	inheritance;

	{
		File*			file = Unity::Instance()->QueryFile();
		std::wstring	temp_filename;

		if (!file->CreateTempFile(temp_filename)) {
			return gs_empty;
		}

		const WCHAR*cname = temp_filename.c_str();
		file->AddAfterDelete(cname);
		if (!file->WriteToFile(cname, kind_ini)) {
			return gs_empty;
		}

		WCHAR buf[4 * 1024];
		GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), cname);
		dst.m_name.assign(buf);
		if (dst.m_name.size() == 0) {
			return gs_empty;
		}

		GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), cname);
		dst.m_description.assign(buf);

		GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), cname);
		dst.m_default_action.assign(buf);

		{
			GetPrivateProfileString(_T("property"), _T("inheritance"), _T(""), buf, _countof(buf), cname);
			Tokenize(buf,dst.m_inheritance,_T(" \t"));
			if(0){
				//debug
				OutputDebugString(_T("==== Inheritance ===="));
				for (const auto&item : dst.m_inheritance) {
					OutputDebugString(item.c_str());
				}
			}
		}

		{
			std::vector<std::wstring> action_sections;
			action_sections.reserve(16);
			GatherActionSections(action_sections, cname);

			Action action;
			for (const auto& section_name: action_sections) {
				ParseActionSection(action, section_name.c_str(), cname);
				//dst.m_actions.emplace(section_name, action);
				dst.m_actions.emplace_back(action);
			}
		}
	}

	name.assign(dst.m_name);
	m_kinds.emplace(name, dst);
	return const_cast<WCHAR*>(name.c_str());
}

Kind* Kinds::FindKind(const WCHAR* kind_name) {
	auto it=m_kinds.find(kind_name);
	if (it == m_kinds.end()) {
		return nullptr;
	}
	return &(it->second);
}

bool Kinds::GenerateKindCandidates(INT_PTR instance_index) {
	OutputDebugString(_T("GenerateKindCandidates"));
	Unity*instance = Unity::Instance(instance_index);
	if (instance == nullptr) {
		OutputDebugString(_T("@1"));
		return false;
	}
		
	bool			is_multi_select = false;
	std::wstring	first_source_name;
	{
		RefineSearch*	search = instance->QueryRefineSearch();
		const auto		num_selection = search->GetSelectionCount();
		if (num_selection == 0) {
			OutputDebugString(_T("@2"));
			return false;
		}

		for (auto select = 0; select < num_selection; ++select) {
			Candidate* candidate = search->GetSelectedCandidate(select);
			if (candidate == nullptr) {
				OutputDebugString(_T("@2.1"));
				continue;
			}
			//Memo: 最初に選択されたソース名
			OutputDebugString(_T("candidate->m_source_name="));
			OutputDebugString(candidate->m_source_name.c_str());
			first_source_name = candidate->m_source_name;
			break;
		}

		if (first_source_name.empty()) {
			OutputDebugString(_T("@3"));
			return false;
		}
		if (2 <= num_selection) {
			is_multi_select = true;
		}else {
			is_multi_select = false;
		}		
	}	

	//ソース名からディフォルトカインドを取り出す
	auto* source= instance->QuerySources()->FindSource(first_source_name.c_str());
	if (source == nullptr) {
		OutputDebugString(_T("@4"));
		return false;
	}
	const auto &default_kind = source->m_default_kind;

	auto* kind = instance->QueryKinds()->FindKind(default_kind.c_str());
	if (kind == nullptr) {
		OutputDebugString(_T("@5"));
		return false;
	}

	//現在のインスタンスへ候補を追加する
	auto* candidates = Unity::Instance()->QueryCandidates();
	for (const auto& action : kind->m_actions) {
		if (CheckAppendable(is_multi_select, action.m_is_multi_selectable)) {
			candidates->AppendCandidate(first_source_name.c_str(),action.m_name.c_str());
		}		
	}
	OutputDebugString(_T("Finish"));
	return true;
}