#include"stdafx.h"

static WCHAR	gs_empty[] = { 0 };


void Kinds::Clear() {
	m_kinds.clear();
}

WCHAR* Kinds::Create(const WCHAR* kind_ini) {
	static std::wstring	name;	//staticについて: 秀丸エディタへ文字列を返すため静的なメモリ領域とする
	std::wstring	description;
	std::wstring	default_action;
	std::wstring	inheritance;

	{
		File*			file = Unity::Instance()->QueryFile();
		std::wstring	temp_filename;

		if (!file->CreateTempFile(temp_filename)) {
			return gs_empty;
		}

		const WCHAR*cname = temp_filename.c_str();
		file->AddAfterDelete(cname);
		if (!file->WriteToFile(cname, kind_ini)) {
			return gs_empty;
		}

		WCHAR buf[8*1024];
		GetPrivateProfileString(_T("property"), _T("name"), _T(""), buf, _countof(buf), cname);
		name.assign(buf);
		if (name.size() == 0) {
			return gs_empty;
		}

		GetPrivateProfileString(_T("property"), _T("description"), _T(""), buf, _countof(buf), cname);
		description.assign(buf);

		GetPrivateProfileString(_T("property"), _T("default_action"), _T(""), buf, _countof(buf), cname);
		default_action.assign(buf);

		GetPrivateProfileString(_T("property"), _T("inheritance"), _T(""), buf, _countof(buf), cname);
		inheritance.assign(buf);

		{//[action.*]セクションのパース
			GetPrivateProfileSectionNames(buf, _countof(buf), cname);
			size_t i = 0;
			while (((i+1)<_countof(buf))&&(buf[i]!=0)&&(buf[i+1]!=0)) {
				WCHAR* top = &buf[i];
				OutputDebugString(top);
				i += wcslen(top)+1;//+1で'\0'を読み飛ばす
			}
			//OutputDebugString(_T("==== buf ===="));
			//OutputDebugString(buf);
		}
	}

	return gs_empty;
}
