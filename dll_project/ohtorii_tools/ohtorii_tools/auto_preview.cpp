#include"stdafx.h"




/////////////////////////////////////////////////////////////////////////////
//	AsyncFileReader
/////////////////////////////////////////////////////////////////////////////

AsyncFileReader::AsyncFileReader(const WCHAR *filename, size_t request_read_byte) :
	m_finished(false),
	m_terminate_request(false),
	m_request_read_byte(request_read_byte),
	m_filename(filename)
{
}

void AsyncFileReader::operator()() {
	try {
		Sequence();
	}
	catch (std::exception) {
		//pass
	}
	m_finished = true;
}

void AsyncFileReader::Sequence(){
	if (m_terminate_request) {
		return;
	}

	//読み込んだファイル内容
	std::vector<uint8_t>		fileimage;
	
	LoadFileImage(fileimage);
	if (m_terminate_request) {
		return;
	}
	if (fileimage.size() == 0) {
		return;
	}
	
	ConvertToWideChar(m_fileimage,fileimage);
}

void AsyncFileReader::LoadFileImage(std::vector<uint8_t>&out_fileimage) {
	DebugLog(_T("AsyncFileReader::Do() Statrt"));
	
	size_t	readed_byte = 0;
	
	{
		FILE*fp = nullptr;
		
		if (_wfopen_s(&fp, m_filename.c_str(), _T("rbS")) == 0) {
			//success
		}else{
			return;
		}
		if (fp == nullptr) {
			return;
		}
		if (m_terminate_request) {
			fclose(fp);
			return;
		}
		
		auto element_size = sizeof(std::remove_reference<decltype(out_fileimage.at(0))>::type);
		out_fileimage.resize(m_request_read_byte,0);
		readed_byte = fread_s(&out_fileimage.at(0), out_fileimage.size()*element_size, element_size, out_fileimage.size(), fp);
		fclose(fp);
	}

	//実際読み込んだファイルサイズへバッファを縮小する
	out_fileimage.resize(readed_byte);
	DebugLog(_T("readed_byte=%d"), readed_byte);
}

void AsyncFileReader::ConvertToWideChar(std::wstring&out, const std::vector<uint8_t>&fileimage) {
	if (fileimage.size() == 0) {
		return;
	}

	const auto bom_type		= GetBomType(&fileimage.at(0), fileimage.size());
	const auto bom_byte_size= GetBomSize(bom_type);
	
	switch (bom_type) {
	case UTF_BOM_TYPE::UTF8:
		{
			const char*	str_ptr	= reinterpret_cast<const char*>(&fileimage.at(bom_byte_size));
			const int	str_size= static_cast<int>(fileimage.size() - bom_byte_size);

			const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, str_ptr, str_size, NULL, 0);
			if (size_needed == 0) {
				return;
			}
			out.resize(size_needed, 0);
			const auto success = MultiByteToWideChar(CP_UTF8, 0, str_ptr, str_size, &out.at(0), size_needed);
			if (!success) {
				out.clear();
				return;
			}
		}
		return;
	
	case UTF_BOM_TYPE::UTF16LE:
		out.assign(	reinterpret_cast<const wchar_t*>(&fileimage.at(bom_byte_size)), 
					(fileimage.size()-bom_byte_size)/sizeof(wchar_t));
		return;
	
	case UTF_BOM_TYPE::UTF16BE:
		//とりあえずリトルエンディアンのまま格納して
		out.assign(	reinterpret_cast<const wchar_t*>(&fileimage.at(bom_byte_size)),
					(fileimage.size() - bom_byte_size) / sizeof(wchar_t));
		static_assert(sizeof(std::remove_reference< decltype(out.at(0))>::type)==sizeof(unsigned short),"sizeof(wchar_t) != sizeof(unsigned) short. Must change _byteswap_ushort function.");
		//エンディアンをひっくり返す
		std::for_each(out.begin(), out.end(), [](auto&c) {c=_byteswap_ushort(c); });
		return;

	case UTF_BOM_TYPE::UNKNOWN:
		return;
		//とりあえずshift-jisとする
		//todo いろいろな文字コードへの対応
		{
			const char*	str_ptr = reinterpret_cast<const char*>(&fileimage.at(bom_byte_size));
			const int	str_size= static_cast<int>(fileimage.size() - bom_byte_size);

			const auto size_needed = MultiByteToWideChar(CP_ACP, 0, str_ptr, str_size, NULL, 0);
			if (size_needed == 0) {
				return;
			}
			out.resize(size_needed, 0);
			const auto success = MultiByteToWideChar(CP_ACP, 0, str_ptr, str_size, &out.at(0), size_needed);
			if (!success) {
				out.clear();
				return;
			}
		}
		return;
	
	default:
		break;
	}
}

bool AsyncFileReader::CheckFinish()const {
	return m_finished;
}

void AsyncFileReader::RequestTerminate() {
	m_terminate_request = true;
}

const std::wstring& AsyncFileReader::GetFileImage()const {
	return m_fileimage;
}




/////////////////////////////////////////////////////////////////////////////
//	AutoPreview
/////////////////////////////////////////////////////////////////////////////
AutoPreview::AutoPreview(const WCHAR* filename) :
	m_finished(false),
	m_mode(FILE_OPEN),
	m_file_reader(filename,80*10)
{
}

AutoPreview::~AutoPreview()  {
	Destroy();
}

void AutoPreview::Excute()  {
	switch (m_mode) {
	case FILE_OPEN:
		StartFileReadThread();
		m_mode = FILE_READ;
		break;

	case FILE_READ:
		if (!m_file_reader.CheckFinish()) {
			return;
		}
		m_mode = PREVIEW;
		//pass

	case PREVIEW:
		Preview();
		m_mode = DESTROY;
		break;

	case DESTROY:
		Destroy();
		m_mode = FINISH;
		break;

	case FINISH:
		//pass

	default:
		break;
	}
}

void AutoPreview::Terminate()  {
	Destroy();
}

bool AutoPreview::Finished()  {
	return m_finished;
}

void AutoPreview::StartFileReadThread() {
	std::thread t([&]() {m_file_reader(); });
	m_thread.swap(t);
}

void AutoPreview::Preview() {
	HINSTANCE hinstExe = GetModuleHandle(NULL);
	BOOL(WINAPI* pfnHidemaru_Hidemaru_EvalMacro)(WCHAR* pwsz);
	*(FARPROC*)&pfnHidemaru_Hidemaru_EvalMacro = GetProcAddress(hinstExe, "Hidemaru_EvalMacro");
	if (pfnHidemaru_Hidemaru_EvalMacro == nullptr) {
		return;
	}

	auto format = _T(R"(
call main;
endmacro;

main:
##output_dll=loaddll("HmOutputPane.dll");
if (##output_dll == 0) {
	return;
}
$$text=R"-(%s)-";
##window=dllfunc(##output_dll,"GetWindowHandle",hidemaruhandle(0));
##ret=sendmessage(##window,0x111,1009 ,0);
##ret=dllfunc(##output_dll, "Output",hidemaruhandle(0),$$text);
##ret=sendmessage(##window,0x111,1015 ,0);
freedll ##output_dll;
return;
)");
	wchar_t macro[10 * 1024];
	_snwprintf_s(macro,_TRUNCATE,format,m_file_reader.GetFileImage().c_str());
	pfnHidemaru_Hidemaru_EvalMacro(const_cast<WCHAR*>(macro));

	DebugLog(macro);
}

void AutoPreview::Destroy() {
	m_file_reader.RequestTerminate();
	if (m_thread.joinable()) {
		m_thread.join();
	}
	m_finished = true;
}


	
