#include "stdafx.h"


void Tokenize(std::vector<std::wstring>& tokens, const std::wstring& str, const std::wstring& delimiters)
{
	tokens.clear();
	// Skip delimiters at beginning.
	std::wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::wstring::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::wstring::npos != pos || std::wstring::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}


std::wstring TrimString(const std::wstring& src, const WCHAR*trimCharacterList)
{
	if (src.size() == 0) {
		return _T("");
	}
	const std::wstring::size_type left = src.find_first_not_of(trimCharacterList);
	const std::wstring::size_type right = src.find_last_not_of(trimCharacterList);
	if (left == right) {
		// src=="\n"
		return _T("");
	}

	if (left == std::wstring::npos) {
		if (right == std::wstring::npos) {
			return src;
		}
		else {
			//"012__"
			return src.substr(0, right + 1);
		}
	}
	else {
		if (right == std::wstring::npos) {
			//"__234"
			return src.substr(left, src.size() - left);
		}
		else {
			//"__234__"
			return src.substr(left, right - left + 1);
		}
	}
}
