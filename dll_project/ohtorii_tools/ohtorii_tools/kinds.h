#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class Action {
public:
	Action();
	//Action();

	///表示に使用する名前
	std::wstring		m_name;
	///秀丸マクロのラベル名
	std::wstring		m_label;
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
			const std::vector<std::wstring>	&base_kind);
	Action* FindAction(const WCHAR* action_name);
	size_t	FindActionIndex(const WCHAR* action_name);
	std::wstring				m_name;
	std::wstring				m_description;
	std::wstring				m_default_action;
	std::vector<std::wstring>	m_base_kind;
	/// m_actions[アクション名]=Action;
	//std::unordered_map<std::wstring, Action>		m_actions;
	//並び順が重要なので std::vector<> を利用
	std::vector<Action>		m_actions;
};

class Kinds {
public:
	//Kinds();
	void Clear();
	
	WCHAR* Create(const WCHAR* kind_ini);
	
	///カインドを見付ける
	Kind* FindKind(const WCHAR* kind_name);
	
	///カインドを見付ける
	size_t FindKindIndex(const WCHAR* kind_name);

	///選択からカインドの種類を生成する
	bool GenerateKindCandidates(INT_PTR instance_index);

	/// 秀丸のラベル名を取得する
	const WCHAR* GetDefaultActionLabelName(const WCHAR* kind_name);

	// アクションのインデックスを取得する
	size_t FindActionIndex(size_t kind_index, const WCHAR* action_name);

	/***********************************************************************
		Kind
	***********************************************************************/
	const WCHAR* GetKindName(size_t kind_index);
	const WCHAR* GetKindDescription(size_t kind_index);
	const WCHAR* GetDefaultAction(size_t kind_index);

	/***********************************************************************
		Action
	***********************************************************************/
	const WCHAR* GetActionName(size_t kind_index, size_t action_index);
	const WCHAR* GetActionLabelName(size_t kind_index, size_t action_index);
	const WCHAR* GetActionDescription(size_t kind_index, size_t action_index);
	bool IsActionQuit(size_t kind_index, size_t action_index);
	bool IsActionMultiSelectable(size_t kind_index, size_t action_index);

private:
	///
	std::vector<Kind>		m_kinds;
};

