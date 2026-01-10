#pragma once
#include<vector>
#include<string>
#include<thread>



class File{
public:
	File();
	~File();
	void Initialize(const WCHAR* temp_directry_name);
	bool RegistAfterDelete(const WCHAR*filename);
	bool UnRegistAfterDelete(const WCHAR*filename);
	/// <summary>
	/// 一時ファイルを生成しそのファイル名を返します
	/// </summary>
	/// <param name="out"></param>
	/// <returns></returns>
	bool CreateTempFile(std::wstring&out);
	bool WriteToFile(const WCHAR* filename, const WCHAR* string);
	bool ReadFile(std::wstring &outFileimage, const WCHAR* filename);
	/// <summary>
	/// 一時ディレクトリ名を取得する
	/// </summary>
	/// <returns>ディレクトリ区切りで終端した文字列</returns>
	const std::wstring& GetTempDirectory()const;

	struct EnumeFileResult {
		std::wstring	m_abs_filename;
	};
	typedef std::deque<EnumeFileResult> EnumeFileResultContainer;
	///指定ディレクトリのファイルを絶対パスで列挙する
	static bool EnumeFiles(EnumeFileResultContainer&out_files, const WCHAR*root_dir, const WCHAR* extension);
	static void Destroy();

protected:
private:
	struct DeleteFile_ {
		DeleteFile_();
		DeleteFile_(const std::wstring&filename);
		void DeleteFile();
        const std::wstring& GetFileName() const { return m_filename; }
	private:
		bool			m_deleting;
		std::wstring	m_filename;
	};
    static std::wstring     m_temp_directory_name;
	static std::deque<DeleteFile_>	m_after_delete;
};
