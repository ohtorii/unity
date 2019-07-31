#pragma once
#include<string>
#include<vector>


class Unity;



/**カインドの継承情報の生成と格納
*/
class Inheritance {
private:
	struct ReferenceCounter {
		ReferenceCounter(){
			m_counter=0;
		};
		ReferenceCounter(const std::wstring&kind_name, INT_PTR counter):
			m_kind_name(kind_name),
			m_counter(counter) 
		{
		}
		std::wstring	m_kind_name;
		INT_PTR			m_counter;
	};

	struct ResolveAction {
		ResolveAction() {
			m_action_index=UNITY_NOT_FOUND_INDEX;
		};
		ResolveAction(const std::wstring&kind_name, size_t action_index, const std::wstring&action_name):
			m_kind_name(kind_name),
			m_action_index(action_index),
			m_action_name(action_name)
		{

		};
		/**カインド名
		(e.g.)	"file_mru"
				"common"
		*/
		std::wstring	m_kind_name;

		/*アクションへのインデックス
		Kind::m_actions[m_action_index];
		*/
		size_t			m_action_index;

		/**アクション名(debug)
		Kind::m_actions[m_action_index]==m_action_name;

		(e.g.)	"preview"
				"append"
		*/
		std::wstring	m_action_name;
	};

	///ディフォルトアクション
	struct DefaultAction {
		///カインド名（秀丸マクロのファイル名になる）
		std::wstring	m_kind_name;
		///ラベル名（秀丸マクロのラベル名）
		std::wstring	m_label_name;
	};

public:	
	Inheritance(Unity*instance);

	//--------------------------------------------------------------------------
	//		ResolveActions
	//--------------------------------------------------------------------------
	bool GenerateResolveActions();
	
	///アクションの個数を取得する
	size_t GetActionCount()const;
	
	///アクションのカインド名を取得する
	const WCHAR*	GetKindName(size_t action_index)const;
	
	///アクションのインデックスを取得する
	size_t			GetActionIndex(size_t action_index)const;
	
	///アクション名を取得する
	const WCHAR*	GetActionName(size_t action_index)const;

	///継承を考慮したアクションのリストを返す
	const std::vector<ResolveAction>&	GetResolveActions()const;

	//--------------------------------------------------------------------------
	//		DefaultAction
	//--------------------------------------------------------------------------
	///ディフォルトアクションを生成する
	bool GenerateDefaultAction(const WCHAR* source_name);
	///ディフォルトアクションの取得
	DefaultAction&	GetDefaultAction();
private:
	Unity	*m_instance;

	//--------------------------------------------------------------------------
	//		ResolveActions
	//--------------------------------------------------------------------------
	//memo: 並び順が重要なのでstd::vector<>を利用
	void FindCommonKind(std::vector<std::wstring> &out_common_kinds);	
	void FindCommonKindRecursive(std::vector<ReferenceCounter>	&out_reference_counter, const std::wstring&kind_name);
	void MakeResolveActions(std::vector<std::wstring> &common_kinds);
	/**アクションの候補一覧*/
	std::vector<ResolveAction>	m_resolve_actions;

	//--------------------------------------------------------------------------
	//		DefaultAction
	//--------------------------------------------------------------------------	
	bool FindDefaultActionNameRecursive(std::wstring&out_default_action_name, const WCHAR* kind_name);
	//toto: 適切なメソッド名へ変更する
	bool GenerateDefaultActionRecursive2(const WCHAR* kind_name, const std::wstring&default_action);
	///ディフォルトアクションの情報
	DefaultAction				m_default_action;
};

