#pragma once

#include <unordered_map>
#include <string>


class Source{
public:
	Source();
	Source(const std::wstring&name, const std::wstring&description, const std::wstring&default_kind, const std::wstring&default_action, const std::wstring&candidate_type, bool is_interactive);
	~Source();

	template<class Archive> void serialize(Archive & archive) {
		archive(
			m_name, 
			m_description,
			m_default_kind,
			m_default_action,
			m_candidate_type,
			m_is_interactive);
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
	/*const*/ std::wstring		m_default_action;
	///候補の種類
	/*const*/ std::wstring		m_candidate_type;
	bool						m_is_interactive;
};


class Sources{
public:
	Sources();
	~Sources();
	template<class Archive> void serialize(Archive & archive) {
		archive(m_sources, m_file_name_to_source_name);
	};

    ///ソースのルートパスを設定する
    void SetRootPath(const WCHAR* root_dir);
	
	///ソースのルートパスを取得する
	/// \で終端している
	const std::wstring& GetRootPath() const;
	
	///指定ディレクトリから全ソースを読み込む
	bool LoadSourceAll(const WCHAR* root_dir);
    
	///INIイメージからソースを作成する
	std::wstring Create(const WCHAR*source_name,const WCHAR* ini_image);

	///ソースを見付ける
	Source* FindSource(const WCHAR*source_name);
	//const WCHAR* GetCandidateType(const WCHAR*source_name);

	///ファイル名とソース名のペアを登録する
	bool AppendFileNameAndSourceName(const WCHAR*file_name, const WCHAR*source_name);
	///ファイル名が登録されているかどうか
	bool ExistFileName(const WCHAR*file_name)const;
	///ファイル名からソース名を得る
	const WCHAR* FileNameToSourceName(const WCHAR*file_name)const;
	//ソース名からファイル名を得る
	const WCHAR* SourceNameToFileName(const WCHAR*source_name)const;

protected:
private:
	//Sources(const Sources&);
	//Sources &operator=(const Sources&);

    std::wstring m_root_path;
	///ソースの情報(Ex. m_sources["file_mru"].name="file_mru")
	std::unordered_map<std::wstring, Source> 	m_sources;
	
	/**ファイル名からソース名を取得するテーブル
	
	（マップの内容）
	"c:\sources\file_mru" : "file_mru",
	"c:\sources\process"  : "process",
	*/
	std::unordered_map<std::wstring, std::wstring> 	m_file_name_to_source_name;
	
	static bool IniToSource(Source&dst, const WCHAR* source_name, const WCHAR*ini_filename);
	static bool MakeSourcePathName(std::wstring&out, const WCHAR*file_name);
	bool Exist(const WCHAR*source_name)const;
};

