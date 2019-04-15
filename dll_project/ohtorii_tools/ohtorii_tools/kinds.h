#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class Action {
public:
	Action();
	//Action();

	///表示に使用する名前
	//std::wstring		m_name;
	///秀丸マクロのラベル名
	std::wstring		m_function;
	///アクションの説明
	std::wstring		m_description;
	///アクション実行後に終了するかどうか
	bool				m_is_quit;
	///アクションを複数選択に対して実行するかどうか
	bool				m_is_multi_selectable;

};

class Kind {
public:
	Kind();
	Kind(	const std::wstring				&name,
			const std::wstring				&description,
			const std::wstring				&default_action,
			const std::vector<std::wstring>	&inheritance);

	std::wstring				m_name;
	std::wstring				m_description;
	std::wstring				m_default_action;
	std::vector<std::wstring>	m_inheritance;
	/// m_actions[アクション名]=Action;
	std::unordered_map<std::wstring, Action>		m_actions;
};

class Kinds {
public:
	//Kinds();
	void Clear();
	WCHAR* Create(const WCHAR* kind_ini);

private:
	/// m_kind[カインド名]=Kind;
	std::unordered_map<std::wstring, Kind>		m_kinds;
};

