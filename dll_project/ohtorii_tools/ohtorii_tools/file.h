#pragma once


class File{
public:
	File();
	~File();
	void AddAfterDelete(const WCHAR*filename);
	bool CreateTempFile(std::wstring&out);
	bool WriteToFile(const WCHAR* filename, const WCHAR* string);

protected:
private:
	File(const File&);
	File &operator=(const File&);
};
