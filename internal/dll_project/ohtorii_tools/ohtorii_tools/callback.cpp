#include"stdafx.h"


/////////////////////////////////////////////////////////////////////////////
//	CallBack
/////////////////////////////////////////////////////////////////////////////
CallBack::CallBack() 
{
}

CallBack::CallBack(const std::wstring&macro_filename, const std::wstring&label_name, const std::wstring&args) :
	m_macro_filename(macro_filename),
	m_label_name(label_name),
	m_args(args)
{
}

void CallBack::Clear() {
	m_macro_filename.clear();
	m_label_name.clear();
	m_args.clear();
}



/////////////////////////////////////////////////////////////////////////////
//	CallBackContainer
/////////////////////////////////////////////////////////////////////////////
bool CallBackContainer::Append(const WCHAR*macro_filename, const WCHAR* label_name, const WCHAR* args) {
	try {
		m_callbackes.emplace_back(macro_filename, label_name, args);
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

void CallBackContainer::Clear() {
	m_callbackes.clear();
}

INT_PTR CallBackContainer::Size()const {
	return m_callbackes.size();
}

const WCHAR* CallBackContainer::GetMacroFileName(INT_PTR index)const {
	try {
		return m_callbackes.at(index).m_macro_filename.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return _T("");
}

const WCHAR* CallBackContainer::GetLabelName(INT_PTR index)const {
	try {
		return m_callbackes.at(index).m_label_name.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return _T("");
}

const WCHAR* CallBackContainer::GetArgs(INT_PTR index)const {
	try {
		return m_callbackes.at(index).m_args.c_str();
	}
	catch (std::exception) {
		//pass
	}
	return _T("");
}
