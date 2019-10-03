#pragma once
#include <functional>
#include <utility>
#include<list>


class TaskBase {
public:
	virtual ~TaskBase() {};
	///実行する
	virtual void Excute()=0;	
	///終了させる
	virtual void Terminate()=0;
	///終了したかどうか調べる
	virtual bool Finished() = 0;
};


///定期的な処理を行うクラス
class RecurringTask {
public:
	typedef std::unique_ptr<TaskBase> TaskType;
	
	RecurringTask();
	~RecurringTask();
	void Destroy();

	///メインスレッドから定期的に呼ぶ処理
	void Update();

	///実行するタスクの登録
	template<typename T, class... _Valty>
	void Register(_Valty&&... _Val) {
		m_tasks.emplace_back(std::make_unique<T>(std::forward<_Valty>(_Val)...));
	}


private:
	RecurringTask(const RecurringTask&);
	RecurringTask& operator=(const RecurringTask&);

	typedef std::list<TaskType> TaskListType;
	TaskListType  m_tasks;
};