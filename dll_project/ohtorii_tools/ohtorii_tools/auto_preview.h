#pragma once
#include<windows.h>
#include<thread>
#include<atomic>
#include<array>
#include"recurring_task.h"



class AsyncFileReader {
public:
	AsyncFileReader(const WCHAR *filename, size_t read_byte);
	void operator()();
	bool CheckFinish()const;
	void RequestTerminate();

	const std::wstring& GetHidemaruScript()const;
private:
	AsyncFileReader(const AsyncFileReader&) = delete;
	AsyncFileReader& operator=(const AsyncFileReader&) = delete;

	void Sequence();
	void LoadFileImage(std::vector<uint8_t>&out_fileimage);
	void ConvertToWideChar(std::wstring&out, const std::vector<uint8_t>&fileimage);
	void ConvertToHidemaruMacro(std::wstring&out, std::wstring&in);

	///読み込むバイト数
	const size_t		m_request_read_byte;
	///読み込むファイル名
	const std::wstring		m_filename;
	///秀丸スクリプト
	std::wstring			m_hidemaru_script;
	///処理が終了したかどうか
	std::atomic<bool>	m_finished;
	///終了リクエスト
	std::atomic<bool>	m_terminate_request;

};




class AutoPreview : public TaskBase {
public:
	AutoPreview(const WCHAR* filename);
	~AutoPreview() override;
	void Excute() override;
	void Terminate() override;
	bool Finished() override;

private:
	AutoPreview(const AutoPreview&) = delete;
	AutoPreview&operator=(const AutoPreview&) = delete;


	void StartFileReadThread();
	void Preview();
	void Destroy();


	enum Mode {
		FILE_OPEN,
		FILE_READ,
		PREVIEW,
		DESTROY,
		FINISH,
	};
	Mode	m_mode;

	AsyncFileReader		m_file_reader;
	std::thread			m_thread;
	std::atomic<bool>	m_finished;

};
