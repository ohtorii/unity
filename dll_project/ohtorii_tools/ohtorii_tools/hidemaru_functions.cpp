#include "stdafx.h"
/*

（方針）
必要とされたときに初めて関数ポインタを取得します。
なので、事前の初期化は必要ありません。

（メモ）
コードは以下のヘルプを参照して作成しました。
秀丸エディタマクロヘルプ→目次→ DLL側から秀丸エディタの関数呼び出し 
*/


HidemaruFunctions::HidemaruFunctions() {
	m_initialized = false;
	pfnHidemaru_Hidemaru_EvalMacro = nullptr;
}

void HidemaruFunctions::Initialize() {
	if (m_initialized) {
		return;
	}
	HINSTANCE hinstExe = GetModuleHandle(NULL);
	if (hinstExe==nullptr) {
		return;
	}

	*(FARPROC*)&pfnHidemaru_Hidemaru_EvalMacro = GetProcAddress(hinstExe, "Hidemaru_EvalMacro");
	
	m_initialized = true;
}

bool HidemaruFunctions::EvalMacro(WCHAR* pwsz) {
	Initialize();
	if (pfnHidemaru_Hidemaru_EvalMacro) {
		return pfnHidemaru_Hidemaru_EvalMacro(pwsz);
	}
	return false;
}