#pragma once
#include<vector>
#include<string>


class CallBack {
public:
	template<class Archive> void serialize(Archive & archive) {
		archive(m_macro_filename, m_label_name, m_args);
	};
	
	CallBack();
	CallBack(const std::wstring&macro_filename, const std::wstring&label_name, const std::wstring&args);
	void Clear();

	std::wstring	m_macro_filename;
	std::wstring	m_label_name;
	std::wstring	m_args;
};


class CallBackContainer {
public:
	template<class Archive> void serialize(Archive & archive) {
		archive(m_callbackes);
	};

	bool Append(const WCHAR*macro_filename, const WCHAR* label_name, const WCHAR* args);
	void Clear();
	INT_PTR Size()const;
	const WCHAR* GetMacroFileName(INT_PTR index)const;
	const WCHAR* GetLabelName(INT_PTR index)const;
	const WCHAR* GetArgs(INT_PTR index)const;

private:
	std::vector<CallBack>	m_callbackes;
};
