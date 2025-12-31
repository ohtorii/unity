#pragma once
#include<vector>
#include<string>
#include<thread>



class File{
public:
	File();
	~File();
	bool RegistAfterDelete(const WCHAR*filename);
	bool UnRegistAfterDelete(const WCHAR*filename);
	bool CreateTempFile(std::wstring&out);
	bool WriteToFile(const WCHAR* filename, const WCHAR* string);
	
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
	static std::deque<DeleteFile_>	m_after_delete;
};
