#pragma once
#include<vector>
#include<string>
#include<thread>



class File{
public:
	File();
	~File();
	bool RegistAfterDelete(const WCHAR*filename);
	bool CreateTempFile(std::wstring&out);
	bool WriteToFile(const WCHAR* filename, const WCHAR* string);
	
	///指定ディレクトリのファイルを絶対パスで列挙する
	static bool EnumeFiles(std::deque<std::wstring>&out_files, const WCHAR*root_dir, const WCHAR* extension);

	static void StartDestroy();
	static void JoinDestroy();

protected:
private:
	File(const File&);
	File &operator=(const File&);

	struct DeleteFile_ {
		DeleteFile_();
		DeleteFile_(const std::wstring&filename);
		
		void DeleteFile();
		void DeleteFileAsThread();
		void Join();	

	private:
		bool			m_deleting;
		std::wstring	m_filename;
		std::thread		m_thread;

	private:
		DeleteFile_(const DeleteFile_&);
		DeleteFile_&operator=(const DeleteFile_&);

	};
	static std::deque<DeleteFile_>	m_after_delete;
};
