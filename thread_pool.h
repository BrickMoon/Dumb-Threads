#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "worker.h"

class ThreadPool{
public:
	ThreadPool();
	~ThreadPool();
	bool Initialize();
	bool Initialize(unsigned num_threads);
	bool Release();
	void SubmitTask(const Task &task, const string &args);
private:
	Worker *m_workers;
	unsigned m_num_threads;
};

#endif //THREAD_POOL_H_
