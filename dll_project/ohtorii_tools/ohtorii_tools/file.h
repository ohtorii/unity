#pragma once
#include<vector>
#include<string>



class File{
public:
	File();
	~File();
	bool RegistAfterDelete(const WCHAR*filename);
	bool CreateTempFile(std::wstring&out);
	bool WriteToFile(const WCHAR* filename, const WCHAR* string);
	static void Destroy();

protected:
private:
	File(const File&);
	File &operator=(const File&);

	static std::vector<std::wstring>	m_after_delete;
};
