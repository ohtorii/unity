#pragma once
#include<string>
#include<vector>
#include<unordered_map>


class UserData {
public:
	/**候補のユーザーデータを設定する（文字列版）
	同一キーがある場合は上書きします。
	return bool	true	成功
				false	失敗
	*/
	template<class Archive> void serialize(Archive & archive) {
		archive(m_string);
	};
	bool			SetUserData(const WCHAR* key, const WCHAR*data);
	const WCHAR*	GetUserData(const WCHAR* key, const WCHAR*	default_data = _T(""))const;
private:
	///ユーザーデータ（文字列）
	std::unordered_map<std::wstring, std::wstring>	m_string;
};
