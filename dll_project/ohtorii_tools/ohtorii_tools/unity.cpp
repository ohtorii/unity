#include"stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//	global variable
///////////////////////////////////////////////////////////////////////////////
static WCHAR	gs_empty[] = { 0 };

std::array<std::shared_ptr<Unity>, UNITY_MAX_CONTEXT_NUM>	Unity::m_instances{nullptr, };
size_t					Unity::m_current_instance_index = 0;
Sources					Unity::m_sources;
Kinds					Unity::m_kinds;
StaticStatus					Unity::m_static_status;



///////////////////////////////////////////////////////////////////////////////
//	static function
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//	class Unity
///////////////////////////////////////////////////////////////////////////////
std::weak_ptr<Unity> Unity::Instance(size_t index) {
	if (index == UNITY_USE_CURRENT_INSTANCE) {
		if (m_instances.at(m_current_instance_index) == nullptr) {
			m_instances.at(m_current_instance_index) = std::make_shared<Unity>();
		}
		return m_instances.at(m_current_instance_index);
	}
	return m_instances.at(index);
}

bool Unity::PushContext(bool exist_context_then_delete) {
	DebugLog(_T("Unity::PushContext(%d)"), m_current_instance_index);
	if (m_instances.size() <= (m_current_instance_index+1)) {
		DebugLog(_T("Unity::PopContext -> m_instances.size() <= (m_current_instance_index+1)"));
		return false;
	}
	++m_current_instance_index;

	if (m_instances.at(m_current_instance_index)) {
		if (exist_context_then_delete) {
			m_instances[m_current_instance_index].reset();
			m_instances[m_current_instance_index] = std::make_shared<Unity>();
		}else {
			//pass
		}
	}else {
		m_instances[m_current_instance_index] = std::make_shared<Unity>();
	}
	return true;
}

bool Unity::PopContext(bool exist_context_then_delete) {
	DebugLog(_T("Unity::PopContext(%d)"), m_current_instance_index);
	if (m_current_instance_index == 0) {
		//これ以上popできない
		DebugLog(_T("Unity::PopContext -> m_current_instance_index == 0"));
		return false;
	}
	if (exist_context_then_delete) {
		m_instances.at(m_current_instance_index).reset();
		m_instances[m_current_instance_index] = nullptr;
	}
	--m_current_instance_index;
	return true;
}

size_t Unity::GetCurrentInstanceIndex() {
	return m_current_instance_index;
}

bool Unity::SerializeCurrentContext(const WCHAR*out_filename) {
	try {
		std::ofstream os(out_filename, std::ios::binary);
		if (!os) {
			return false;
		}
		
		{
			cereal::BinaryOutputArchive archive(os);
			archive(
				Unity::m_instances,
				Unity::m_current_instance_index,
				Unity::m_sources,
				Unity::m_kinds,
				Unity::m_static_status,
				InterfaceSugar::m_instance);
		}
		os.close();
		return true;
	}catch (std::exception) {
		//pass
	}
	return false;
}

bool Unity::DeSerializeToCurrentContext(const WCHAR*input_filename) {
	try {
		std::ifstream is(input_filename, std::ios::binary);
		if (!is) {
			return false;
		}
		{
			cereal::BinaryInputArchive	archive(is);
			archive(
				Unity::m_instances, 
				Unity::m_current_instance_index, 
				Unity::m_sources,
				Unity::m_kinds, 
				Unity::m_static_status, 
				InterfaceSugar::m_instance);
		}
		is.close();
		return true;
	}
	catch (std::exception) {
		//pass
	}
	return false;
}

 bool Unity::SerializeStaticStatusContext(const WCHAR*out_filename) {
	 try {
		 std::ofstream os(out_filename, std::ios::binary);
		 if (!os) {
			 return false;
		 }

		 {
			 cereal::BinaryOutputArchive archive(os);
			 archive(Unity::m_static_status/*, Unity::Instance().lock()->m_candidates*/);
		 }
		 os.close();
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

 bool Unity::DeSerializeToStaticStatusContext(const WCHAR*input_filename) {
	 try {
		 std::ifstream is(input_filename, std::ios::binary);
		 if (!is) {
			 return false;
		 }
		 {
			 cereal::BinaryInputArchive	archive(is);
			 archive(Unity::m_static_status/*, Unity::Instance().lock()->m_candidates*/);
		 }
		 is.close();
		 return true;
	 }
	 catch (std::exception) {
		 //pass
	 }
	 return false;
}

void Unity::Destroy() {
	for (size_t i = 0; i<m_instances.size() ; ++i) {
		auto&item = m_instances.at(i);
		if (item==nullptr) {
			continue;
		}
		item->QueryASyncFiles().Destroy();
		item.reset();
	}
	m_kinds.Clear();
	m_current_instance_index = 0;

	//
	//memo: ASyncFileでファイルを閉じた後で終了処理を行う。
	//そうしないと、ASyncFileがファイルを開いているためファイルの削除が出来ない。
	//
	//今のところ削除するファイルは1個でパフォーマンスに大きな影響は無いため対策は行わないことにした。
	//
	File::StartDestroy();
	File::JoinDestroy();
}

Sources& Unity::QuerySources(){
	return m_sources;
}

File&	Unity::QueryFile(){
	return m_file;
}

Candidates&	Unity::QueryCandidates() {
	return m_candidates;
}

RefineSearch&	Unity::QueryRefineSearch() {
	return m_refine_search;
}

Kinds&			Unity::QueryKinds() {
	return m_kinds;
}

Inheritance&	Unity::QueryInheritance() {
	return m_inheritance;
}

UserData&		Unity::QueryUserData() {
	return m_user_data;
}

ContextStatus&	Unity::QueryContextStatus() {
	return m_context_status;
}

StaticStatus& Unity::QueryStaticStatus() {
	return m_static_status;
}

ASyncFiles&		Unity::QueryASyncFiles() {
	return m_async_files;
}

RecurringTask&	Unity::QueryRecurringTask() {
	return m_recurring_task;
}

void Unity::ChangeCandidates() {
	m_changed_candidates = true;
}

bool Unity::HasChangedCandidates() {
	return m_changed_candidates;
}

bool Unity::ClearChangedCandidatesAndReturnPrevStatus() {
	auto prev = m_changed_candidates;
	m_changed_candidates = false;
	return prev;
}


bool Unity::StatusUpdate(const WCHAR*kind_name, const WCHAR*action_name, INT_PTR context_index) {
	auto kind_index = m_kinds.FindKindIndex(kind_name);
	if (kind_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}

	auto action_index = m_kinds.FindActionIndex(kind_index, action_name);
	if (action_index == UNITY_NOT_FOUND_INDEX) {
		return false;
	}
	m_static_status.UpdateStatus(kind_index, action_index);
	m_context_status.UpdateStatus(kind_index, action_index, context_index);
	return true;
}

Unity::Unity() : 
	m_refine_search(this), 
	m_inheritance(this),
	m_async_files(this)
{
	m_changed_candidates = false;
}

Unity::~Unity(){

}

class AsyncFileReader {
public:
	//AsyncFileReader() {
	//}

	AsyncFileReader(const WCHAR *filename): 
		m_finished(false), 
		m_terminate_request(false), 
		m_filename(filename)
	{
	}

	void operator()() {
		Do();
		m_finished = true;
	}

	bool CheckFinish()const {
		return m_finished;
	}

	void RequestTerminate() {
		m_terminate_request = true;
	}

private:
	AsyncFileReader(const AsyncFileReader&) = delete;
	AsyncFileReader& operator=(const AsyncFileReader&) = delete;

	void Do() {
		if (m_terminate_request) {
			return;
		}

		std::array<char, 80 * 10>	buf;
		buf[0] = 0;

		{
			FILE*fp = nullptr;
			if (fopen_s(&fp, "", "rb") != 0) {
				return;
			}

			if (m_terminate_request) {
				fclose(fp);
				return;
			}

			fread_s(&buf[0], sizeof(buf), 1, sizeof(buf), fp);
			fclose(fp);
		}

		if (m_terminate_request) {
			return;
		}

		//UCS2と仮定して処理を進める
		//todo : 文字コード変換への対応
		//		https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
		m_fileimage.assign(reinterpret_cast<wchar_t*>(&buf[0]));
		
		//todo 秀丸エディタで実行するコマンドを作る
	}

	///読み込むファイル名
	const std::wstring	m_filename;
	///読み込んだファイル内容
	std::wstring	m_fileimage;
	///処理が終了したかどうか
	std::atomic<bool>	m_finished;
	///終了リクエスト
	std::atomic<bool>	m_terminate_request;

};


class AutoPreview : public TaskBase{
public:
	AutoPreview(const WCHAR* filename) : 
		m_finished(false),
		m_mode(FILE_OPEN),
		m_file_reader(filename)
	{
	}
	
	~AutoPreview() override{
		Destroy();
	}

	void Excute() override{
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
	
	void Terminate() override{
		Destroy();
	}

	bool Finished() override{
		return m_finished;
	}

private:
	void StartFileReadThread() {
		std::thread t(std::ref(m_file_reader));
		m_thread.swap(t);
	}

	void Preview() {
		HINSTANCE hinstExe = GetModuleHandle(NULL);
		BOOL(WINAPI* pfnHidemaru_Hidemaru_EvalMacro)(WCHAR* pwsz);
		*(FARPROC*)&pfnHidemaru_Hidemaru_EvalMacro = GetProcAddress(hinstExe, "Hidemaru_EvalMacro");
		if (pfnHidemaru_Hidemaru_EvalMacro == nullptr) {
			return ;
		}

		DebugLog(_T("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		auto macro = _T(R"(
call main;
endmacro;

main:
	##output_dll=loaddll("HmOutputPane.dll");
	if (##output_dll == 0) {
		return;
	}
	$$text="test";
	##window=dllfunc(##output_dll,"GetWindowHandle",hidemaruhandle(0));
	##ret=sendmessage(##window,0x111,1009 ,0);
	##ret=dllfunc(##output_dll, "Output",hidemaruhandle(0),$$text);
	##ret=sendmessage(##window,0x111,1015 ,0);
	freedll ##output_dll;
	return;
)");
		pfnHidemaru_Hidemaru_EvalMacro(const_cast<WCHAR*>(macro));
	}

	void Destroy() {
		m_file_reader.RequestTerminate();
		if (m_thread.joinable()) {
			m_thread.join();
		}
		m_finished = true;
	}


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


bool Unity::AutoPreviewRegist(HWND output_handle, const WCHAR*filename) {
	m_recurring_task.Register<AutoPreview>(filename);
	return true;
}
