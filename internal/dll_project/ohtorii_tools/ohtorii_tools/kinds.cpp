#include"stdafx.h"



///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static 			WCHAR	gs_empty[]		= { 0 };
static const	WCHAR	*gs_prefix		=_T("action.");
static const	size_t	gs_prefix_size	= 7;


///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////
static bool ContainsAction(const WCHAR*str) {
	return std::equal(gs_prefix, gs_prefix + gs_prefix_size, str);
}

static bool StringToBool(const WCHAR* str) {
	if (_wcsicmp(str, _T("false")) == 0) {
		return false;
	}
	if (_wcsicmp(str, _T("0")) == 0) {
		return false;
	}
	return true;
}


//[action.*]セクションのパース	
static void GatherActionSections(std::vector<std::wstring> &dst, const WCHAR* filename) {
	WCHAR buf[2 * 1024];
	buf[0] = 0;

	GetPrivateProfileSectionNames(buf, _countof(buf), filename);
	
	size_t i = 0;
	while (((i + 1) < _countof(buf)) && (buf[i] != 0) && (buf[i + 1] != 0)) {
		WCHAR* top = &buf[i];
		if (ContainsAction(top)) {
			dst.push_back(top);
		}
		i += wcslen(top) + 1;//+1で'\0'を読み飛ばす
	}
}

static void ParseActionSection(Action &dst, const WCHAR*section_name, const WCHAR* filename) {
	/*アクションのセクションをパースする
	*/
	WCHAR buf[2*1024];
	buf[0] = 0;

	//memo: "action.nop" -> "nop"
	dst.m_name.assign(section_name+gs_prefix_size);

	GetPrivateProfileString(section_name, _T("function"), _T(""), buf, _countof(buf), filename);
	dst.m_label.assign(buf);

	GetPrivateProfileString(section_name, _T("description"), _T(""), buf, _countof(buf), filename);
	dst.m_description.assign(buf);

	GetPrivateProfileString(section_name, _T("is_quit"), _T("false"), buf, _countof(buf), filename);
	dst.m_is_quit = StringToBool(buf);
	
	GetPrivateProfileString(section_name, _T("is_multi_selectable"), _T("false"), buf, _countof(buf), filename);
	dst.m_is_multi_selectable = StringToBool(buf);
	
	GetPrivateProfileString(section_name, _T("is_start"), _T("false"), buf, _countof(buf), filename);
	dst.m_is_start = StringToBool(buf);

	GetPrivateProfileString(section_name, _T("is_edit"), _T("false"), buf, _countof(buf), filename);
	dst.m_is_edit = StringToBool(buf);

	GetPrivateProfileString(section_name, _T("is_reget_candidates"), _T("false"), buf, _countof(buf), filename);
	dst.m_is_reget_candidates = StringToBool(buf);
	
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
	m_is_start = false;
	m_is_edit = false;
	m_is_reget_candidates = false;
}


///////////////////////////////////////////////////////////////////////////////
//	class Kind
///////////////////////////////////////////////////////////////////////////////
Kind::Kind() {

}

Kind::Kind(const std::wstring		&name,
	const std::wstring				&description,
	const std::wstring				&default_action,
	const std::vector<std::wstring>	&base_kind):
	m_name(name),
	m_description(description),
	m_default_action(default_action),
	m_base_kind(base_kind)
{

}

Action* Kind::FindAction(const WCHAR* action_name) {
	/*size_t size = m_actions.size();
	for (size_t i = 0; i < size; ++i) {
		auto&item=m_actions.at(i);
		if (wcscmp(item.m_name.c_str(), action_name) == 0) {
			return &m_actions.at(i);
		}
	}*/
	auto index = FindActionIndex(action_name);
	if (index == UNITY_NOT_FOUND_INDEX) {
		return nullptr;
	}
	return &m_actions.at(index);
}

size_t	Kind::FindActionIndex(const WCHAR* action_name) {
	size_t size = m_actions.size();
	for (size_t i = 0; i < size; ++i) {
		auto&item = m_actions.at(i);
		if (wcscmp(item.m_name.c_str(), action_name) == 0) {
			return i;
		}
	}
	return UNITY_NOT_FOUND_INDEX;
}

///////////////////////////////////////////////////////////////////////////////
//	class Kinds
///////////////////////////////////////////////////////////////////////////////
void Kinds::Clear() {
	m_kinds.clear();
}

WCHAR* Kinds::Create(const WCHAR* kind_ini) {
	Kind dst;

	DebugLog(_T("Kinds::Create"));

	{
		auto&			file = Unity::Instance().lock()->QueryFile();
		std::wstring	temp_filename;

		if (!file.CreateTempFile(temp_filename)) {
			DebugLog(_T("  return false@1"));
			return gs_empty;
		}

		const WCHAR*ini_filename = temp_filename.c_str();
		if (!file.WriteToFile(ini_filename, kind_ini)) {
			DebugLog(_T("  return false@2"));
			return gs_empty;
		}

		if (!IniToKind(dst, ini_filename)) {
			return gs_empty;
		}
	}
			
	m_kinds.emplace_back(dst);
	DebugLog(_T("  return=%s"), m_kinds.back().m_name.c_str());
	return const_cast<WCHAR*>(m_kinds.back().m_name.c_str());
}

Kind* Kinds::FindKind(const WCHAR* kind_name) {	
	auto it = std::find_if(
				m_kinds.begin(), 
				m_kinds.end(), 
				[kind_name](const auto&item) {return item.m_name.compare(kind_name) == 0; }
				);
	if (it == m_kinds.end()) {
		return nullptr;
	}
	return &(*it);
}

size_t Kinds::FindKindIndex(const WCHAR* kind_name) {
	const auto num = m_kinds.size();
	for (size_t i = 0; i < num; ++i) {
		if (m_kinds.at(i).m_name.compare(kind_name) == 0) {
			return i;
		}
	}
	return UNITY_NOT_FOUND_INDEX;
}

size_t Kinds::FindActionIndex(size_t kind_index, const WCHAR* action_name) {
	try {
		const auto & actions = m_kinds.at(kind_index).m_actions;
		const auto num = actions.size();
		for (size_t i = 0; i < num; ++i) {
			if (actions.at(i).m_name.compare(action_name) == 0) {
				return i;
			}
		}
	}
	catch (std::exception) {
		//pass
	}
	return UNITY_NOT_FOUND_INDEX;
}

/****************************************************************************
	Kind
****************************************************************************/
const WCHAR* Kinds::GetKindName(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_name.c_str();
	}catch (std::exception) {
		//pass
	}
	return gs_empty;
}

const WCHAR* Kinds::GetKindDescription(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_description.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}

const WCHAR* Kinds::GetDefaultAction(size_t kind_index) {
	try {
		return m_kinds.at(kind_index).m_default_action.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}

/****************************************************************************
	Action
****************************************************************************/
const WCHAR* Kinds::GetActionName(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
const WCHAR* Kinds::GetActionLabelName(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_label.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
const WCHAR* Kinds::GetActionDescription(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_description.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return gs_empty;
}
bool Kinds::IsActionQuit(size_t kind_index, size_t action_index)const {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_quit;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
bool Kinds::IsActionMultiSelectable(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_multi_selectable;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
bool Kinds::IsActionStart(size_t kind_index, size_t action_index)const {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_start;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
bool Kinds::IsEdit(size_t kind_index, size_t action_index) {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_edit;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}
bool Kinds::IsRegetCandidates(size_t kind_index, size_t action_index)const {
	try {
		return m_kinds.at(kind_index).m_actions.at(action_index).m_is_reget_candidates;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

const WCHAR* Kinds::GetDefaultActionLabelName(const WCHAR* kind_name){
	DebugLog(_T("GetHidemaruLabelName"));
	auto*kind = FindKind(kind_name);
	if (kind == nullptr) {
		DebugLog(_T("kind == nullptr"));
		return gs_empty;
	}
	DebugLog(_T("kind->m_default_action="));
	DebugLog(kind->m_default_action.c_str());
	auto *action = kind->FindAction(kind->m_default_action.c_str());
	if (action == nullptr) {
		DebugLog(_T("action == nullptr"));
		return gs_empty;
	}
	DebugLog(_T("action->m_function="));
	DebugLog(action->m_label.c_str());
	return action->m_label.c_str();
}

bool Kinds::GenerateKindCandidates(INT_PTR instance_index) {
	DebugLog(_T("GenerateKindCandidates"));
	std::weak_ptr<Unity> instance = Unity::Instance(instance_index);
	if (instance.expired()) {
		DebugLog(_T("  return false @1"));
		return false;
	}	
	
	{
		auto&inheritance = instance.lock()->QueryInheritance();
		if (! inheritance.GenerateResolveActions()) {
			DebugLog(_T("  return false @2"));
			return false;
		}

		auto& candidates = Unity::Instance().lock()->QueryCandidates();
		//現在のインスタンスへ候補を追加する
		for (const auto&item : inheritance.GetResolveActions()) {
			DebugLog(_T("  item.m_kind_name.c_str()=%s"),item.m_kind_name.c_str());
			auto* kind = instance.lock()->QueryKinds().FindKind(item.m_kind_name.c_str());
			if (kind == nullptr) {
				continue;
			}
			const auto & action = kind->m_actions.at(item.m_action_index);
						
			auto candidate_index = candidates.AppendCandidate(_T("action"), action.m_name.c_str(), (item.m_kind_name+_T("\t")+action.m_description).c_str());
			candidates.SetUserData(candidate_index, _T("__kind__"), kind->m_name.c_str());
		}
			
		DebugLog(_T("  return true"));
		return true;

	}
}

bool Kinds::IniToKind(Kind&dst,const WCHAR*ini_filename){		
	WCHAR buf[16 * 1024];
	
	GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_name.assign(buf);	
	if (dst.m_name.size() == 0)	
	{
		WCHAR fname[_MAX_FNAME];
		const errno_t err = _wsplitpath_s(ini_filename, nullptr, 0, nullptr, 0, fname, _MAX_FNAME, nullptr, 0);
		if (err != 0) {
			return false;
		}
		dst.m_name.assign(fname);		
	}

	GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_description.assign(buf);

	GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), ini_filename);
	dst.m_default_action.assign(buf);

	{
		GetPrivateProfileString(_T("property"), _T("base_kind"), _T(""), buf, _countof(buf), ini_filename);
		Tokenize(dst.m_base_kind, buf, _T(" \t"));
		if (0) {
			//debug
			DebugLog(_T("  ==== Inheritance ===="));
			for (const auto&item : dst.m_base_kind) {
				DebugLog(_T("  %s"), item.c_str());
			}
		}
	}
	
	{
		std::vector<std::wstring> action_sections;
		action_sections.reserve(16);
		GatherActionSections(action_sections, ini_filename);

		Action action;
		for (const auto& section_name : action_sections) {
			ParseActionSection(action, section_name.c_str(), ini_filename);
			dst.m_actions.emplace_back(action);
		}
	}
	return true;
}

bool Kinds::LoadKindAll(const WCHAR* root_dir) {
	File::EnumeFileResultContainer file_names;
	if (!Unity::Instance().lock()->QueryFile().EnumeFiles(file_names, root_dir, _T("*.ini"))) {
		return false;
	}

	const auto num = file_names.size();
	m_kinds.resize(num);
	for (size_t i = 0; i < num; ++i) {
		if (!IniToKind(m_kinds.at(i), file_names.at(i).m_abs_filename.c_str())) {
			return false;
		}
	}
	Dump();
	return true;
}

void Kinds::Dump()const {
	DebugLog(_T("==== Kinds::Dump [kinds.size=%d] ===="), m_kinds.size());
	size_t kind_index = 0;
	for(auto &kind : m_kinds)
	{
		DebugLog(_T("  [%d]%s"), kind_index, kind.m_name.c_str());
		DebugLog(_T("    m_actions.size()=%d"), kind.m_actions.size());
		size_t action_index = 0;
		for (auto&action : kind.m_actions) {
			DebugLog(_T("    [%d]%s"), action_index, action.m_name.c_str());
			DebugLog(_T("       is_edit=%d"),action.m_is_edit);
			DebugLog(_T("       is_quit=%d"), action.m_is_quit);
			DebugLog(_T("       is_is_multi_selectable=%d"), action.m_is_multi_selectable);
			DebugLog(_T("       is_is_start=%d"), action.m_is_start);
			DebugLog(_T("       is_reget_candidates=%d"), action.m_is_reget_candidates);

			++action_index;
		}

		++kind_index;
	}
}