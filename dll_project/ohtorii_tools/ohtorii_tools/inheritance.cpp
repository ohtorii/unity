#include"stdafx.h"


Inheritance::Inheritance(Unity*instance) : m_instance(instance){
	
}

bool Inheritance::Generate() {
	/**選択された候補で利用している共通カインド
	(例)
	common_kinds[0]="common";
	common_kinds[1]="cdable";
	*/
	std::vector<std::wstring> common_kinds;
	FindCommonKind(common_kinds);
	if (true) {
		//debug
		WCHAR	buf[256];
		const auto num = common_kinds.size();
		_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("==== common_kinds ==== (num=%zd)"), num);
		OutputDebugString(buf);
		for (size_t i = 0; i < num ; ++i) {
			_snwprintf_s(buf, _countof(buf),_TRUNCATE, _T("  [%zd/%zd]%s"), i+1,num,common_kinds.at(i).c_str());
			OutputDebugString(buf);
		}
	}
	//m_common_kinds
	return false;
}

void Inheritance:: FindCommonKind(std::vector<std::wstring> &out_common_kinds) {
	out_common_kinds.clear();

	//reference_counter[カインド名]=参照数
	//参照数==num_selectionのアイテムが共通カインドです。
	using si = std::pair<std::wstring, int>;
	std::unordered_map<std::wstring, int>	reference_counter;

	auto		instance		= m_instance;
	const auto	num_selection	= instance->QueryRefineSearch()->GetSelectionCandidateCount();

	{
		auto*refine_search	= instance->QueryRefineSearch();
		auto*candidates		= instance->QueryCandidates();
		auto*sources		= instance->QuerySources();
		auto*kinds			= instance->QueryKinds();
		//
		// 共通カインドを集める
		//
		if(true){
			WCHAR	buf[256];
			_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("num_selection=%zd"), num_selection);
			OutputDebugString(buf);
		}
		for (INT_PTR selected_index = 0; selected_index < num_selection; ++selected_index) {
			auto candidate_index	= refine_search->GetSelectionCandidateIndex(selected_index);
			if (candidate_index == UNITY_NOT_FOUND_INDEX) {
				continue;
			}
			auto*source_name		= candidates->GetSourceName(candidate_index);
			if (!source_name) {
				continue;
			}
			auto*source				= sources->FindSource(source_name);
			if (!source) {
				continue;
			}
			auto*kind				= kinds->FindKind(source->m_default_kind.c_str());
			if (!kind) {
				continue;
			}

			auto it = reference_counter.find(kind->m_name);
			if (it == reference_counter.end()) {
				reference_counter.insert(si{ kind->m_name,1 });
			}
			else {
				++(it->second);
			}
			//todo 基底カインドを再帰的に処理する
			for (const auto &item : kind->m_base_kind) {
				auto it = reference_counter.find(item);
				if (it == reference_counter.end()) {
					reference_counter.insert(si{ item,1 });
				}
				else {
					++(it->second);
				}
			}
		}
	}

	if (true) {
		WCHAR	buf[256];
		_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("==== reference_count ==== (num=%zd)"), reference_counter.size());
		OutputDebugString(buf);
		for (const auto&item : reference_counter) {
			_snwprintf_s(buf, _countof(buf), _TRUNCATE, _T("  %s=%d"), item.first.c_str(), item.second);
			OutputDebugString(buf);
		}
	}
	//
	// 共通のカインドを見付ける
	// 
	for (const auto&item : reference_counter) {
		if (item.second == num_selection) {
			out_common_kinds.emplace_back(item.first);
		}
	}

}