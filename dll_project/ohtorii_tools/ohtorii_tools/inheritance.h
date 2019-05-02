#pragma once
#include<string>
#include<vector>


class Unity;


struct Method{
	/**カインド名
	Ex. "file_mru"
		"common"
	*/
	std::wstring	m_kind;

	/*アクションへのインデックス
	Kind::m_actions[m_action_index];
	*/
	size_t			m_action_index;

	/**アクション名
	Ex. "preview"
		"append"
	*/
	//std::wstring	m_action;	
};

/**カインドの継承情報の生成と格納
*/
class Inheritance {
public:	
	Inheritance(Unity*instance);
	bool Generate();
private:
	void FindCommonKind(std::vector<std::wstring> &out_common_kinds);	
	
	Unity	*m_instance;
	/**アクションの候補一覧
	*/
	std::vector<Method>			m_actions;
};

