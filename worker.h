#ifndef WORKER_H_
#define WORKER_H_

#ifdef _WIN32
#elif MACOS
#else
#include "unix_worker_impl.h"
#endif

class Worker{
public:
	Worker();
	~Worker();
	bool Initialize(int id);
	bool Release();
	void SubmitTask(const Task &task, const string &args);
private:
	WorkerImplementation *m_impl;
};

#endif //WORKER_H_
