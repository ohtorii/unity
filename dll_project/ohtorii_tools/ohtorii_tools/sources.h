#pragma once

#include <unordered_map>
#include <string>


class Source{
public:
	Source();
	Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&candidate_type);
	~Source();
//protected:
//private:
	//Source(const Source&);
	//Source &operator=(const Source&);

	///ソースのファイル名(Ex. file_mru.mac)
	//std::wstring		m_filename;
	///ソース名(Ex. file_mru)
	const std::wstring		m_name;
	///説明(Ex. candidates from file MRU list)
	const std::wstring		m_description;
	///ディフォルトのカインド(Ex. file)
	const std::wstring		m_default_kind;
	///候補の種類
	const std::wstring		m_candidate_type;
};


class Sources{
public:
	Sources();
	~Sources();
	WCHAR* Create(const WCHAR* source_ini);
	const WCHAR* GetCandidateType(const WCHAR*source_name);

protected:
private:
	Sources(const Sources&);
	Sources &operator=(const Sources&);

	///ソースの情報(Ex. m_sources["file_mru"].name="file_mru")
	std::unordered_map<std::wstring, Source> 	m_sources;

	bool Exist(const WCHAR*source_name)const;
};

