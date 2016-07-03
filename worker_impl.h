#ifndef WORKER_IMPL_H_
#define WORKER_IMPL_H_

#include <functional>
#include <string>
#include <queue>
#include <utility>

using namespace std;

class Task{
public:
	Task();
	Task(function<void(const string &)> task);		
	~Task();
	void operator()(const string &args);
private:
	function<void(const string &)> m_task;
};

class WorkerImplementation{
public:
	WorkerImplementation();
	~WorkerImplementation();
	virtual bool Initialize(int id)=0;
	virtual bool Release()=0;
	virtual void SubmitTask(const Task &task, const string &args)=0;
private:
};

#endif //WORKER_IMPL_H_
