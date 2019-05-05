#pragma once
#include<string>
#include<vector>


class Unity;



/**カインドの継承情報の生成と格納
*/
class Inheritance {
public:	
	Inheritance(Unity*instance);
	bool Generate();

private:
	struct ReferenceCounter{
		std::wstring	m_kind;
		INT_PTR			m_counter;
	};

	struct ResolveAction {
		/**カインド名
		(e.g.)	"file_mru"
				"common"
		*/
		std::wstring	m_kind;

		/*アクションへのインデックス
		Kind::m_actions[m_action_index];
		*/
		size_t			m_action_index;

		/**アクション名
		Kind::m_actions[m_action_index]==m_action\name;

		(e.g.)	"preview"
				"append"
		*/
		std::wstring	m_action_name;	
	};

	//memo: 並び順が重要なのでstd::vector<>を利用
	void FindCommonKind(std::vector<std::wstring> &out_common_kinds);	
	void FindCommonKindRecursive(std::vector<ReferenceCounter>	&out_reference_counter, const std::wstring&kind_name);

	void MakeResolveActions(std::vector<std::wstring> &common_kinds);

	Unity	*m_instance;
	/**アクションの候補一覧
	*/
	std::vector<ResolveAction>	m_resolve_actions;
};

