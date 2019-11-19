#include"stdafx.h"



RecurringTask::RecurringTask() {

}

RecurringTask::~RecurringTask() {
	Destroy();
}

void RecurringTask::Destroy() {
	for (auto &task : m_tasks) {
		task->Terminate();
	}
	m_tasks.clear();
}

void RecurringTask::Update() {
	try {
		Main();
	}
	catch (std::exception) {
		//pass
	}
}

void RecurringTask::Main(){
	//タスクを実行する
	for (auto &task:m_tasks) {
		if (task->Finished()) {
			continue;
		}
		task->Excute();
	}

	//終了したタスクを削除する
	auto first = m_tasks.begin();
	auto end = m_tasks.end();
	for (; first != end;) {
		if ((*first)->Finished()) {
			auto target = first;
			++first;
			m_tasks.erase(target);
			//DebugLog(_T("  -> Erase task."));
		}
		else {
			++first;
		}
	}
	//DebugLog(_T("  -> Finish"));
}

