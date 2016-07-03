#ifndef UNIX_WORKER_IMPL_H_
#define UNIX_WORKER_IMPL_H_

#include "worker_impl.h"
#include <pthread.h>

class UnixWorkerImplementation : public WorkerImplementation{
public:
	UnixWorkerImplementation();
	~UnixWorkerImplementation();
	bool Initialize(int id);
	bool Release();
	void SubmitTask(const Task &task, const string &args);
private:
	static void *AcceptTasks(void *args);
	void Kill();
	pthread_t m_thread;
	pthread_mutex_t m_task_queue_mutex;
	pthread_mutex_t m_start_mutex;
	pthread_cond_t m_start_cond;
	pthread_mutex_t m_task_mutex;
	pthread_cond_t m_task_cond;
	pair <void*, int> m_thread_args;
	bool m_is_started;
	bool m_is_running;
	queue<pair<Task, string> > m_task_queue;
};

#endif //UNIX_WORKER_IMPL_
