#pragma once

class HidemaruFunctions {
public:
	HidemaruFunctions();
	bool EvalMacro(WCHAR* pwsz);
private:
	HidemaruFunctions(const HidemaruFunctions&) = delete;
	HidemaruFunctions&operator=(const HidemaruFunctions&) = delete;

	void Initialize();


	bool m_initialized;
	BOOL(WINAPI* pfnHidemaru_Hidemaru_EvalMacro)(WCHAR* pwsz);

};
