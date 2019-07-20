#pragma once

#include <unordered_map>
#include <string>


class Source{
public:
	Source();
	Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&candidate_type);
	~Source();

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_name, 
			m_description,
			m_default_kind,
			m_candidate_type);
	};

//protected:
//private:
	//Source(const Source&);
	//Source &operator=(const Source&);

	///ソースのファイル名(Ex. file_mru.mac)
	//std::wstring		m_filename;
	///ソース名(Ex. file_mru)
	/*const*/ std::wstring		m_name;
	///説明(Ex. candidates from file MRU list)
	/*const*/ std::wstring		m_description;
	///ディフォルトのカインド(Ex. file)
	/*const*/ std::wstring		m_default_kind;
	///候補の種類
	/*const*/ std::wstring		m_candidate_type;
};


class Sources{
public:
	Sources();
	~Sources();
	template<class Archive> void serialize(Archive & archive) {
		archive(m_sources, m_file_name_to_source_name);
	};

	WCHAR* Create(const WCHAR* source_ini);

	///ソースを見付ける
	Source* FindSource(const WCHAR*source_name);
	//const WCHAR* GetCandidateType(const WCHAR*source_name);

	///ファイル名とソース名のペアを登録する
	bool AppendFileNameAndSourceName(const WCHAR*file_name, const WCHAR*source_name);
	///ファイル名が登録されているかどうか
	bool ExistFileName(const WCHAR*file_name)const;
	///ファイル名からソース名を得る
	const WCHAR* FileNameToSourceName(const WCHAR*file_name)const;

protected:
private:
	//Sources(const Sources&);
	//Sources &operator=(const Sources&);

	///ソースの情報(Ex. m_sources["file_mru"].name="file_mru")
	std::unordered_map<std::wstring, Source> 	m_sources;
	///ファオル名からソース名を取得するテーブル
	std::unordered_map<std::wstring, std::wstring> 	m_file_name_to_source_name;
	bool Exist(const WCHAR*source_name)const;
};

