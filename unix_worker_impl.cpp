#include "unix_worker_impl.h"
UnixWorkerImplementation::UnixWorkerImplementation(){
}

UnixWorkerImplementation::~UnixWorkerImplementation(){
}

void *UnixWorkerImplementation::AcceptTasks(void *args){
	pair<void*, int> *data = (pair<void*, int> *)args;
	UnixWorkerImplementation *context = 
	(UnixWorkerImplementation *)data->first;
	int tid = data->second;
	
	context->m_is_running = true;

	pthread_mutex_lock(&context->m_start_mutex);
	context->m_is_started = true;
	pthread_cond_signal(&context->m_start_cond);
	pthread_mutex_unlock(&context->m_start_mutex);
	
	pair<Task, string>p;
	unsigned num_tasks;
	int i;
	while(true){
		pthread_mutex_lock(&context->m_task_mutex);
		while(
			context->m_is_running &&
			context->m_task_queue.size() == 0
		){
			pthread_cond_wait(
				&context->m_task_cond,
				&context->m_task_mutex
			);
		}
		num_tasks = context->m_task_queue.size();
		printf("Number of tasks: %d\n", num_tasks);
		pthread_mutex_unlock(&context->m_task_mutex);
		if(num_tasks > 0){
			pthread_mutex_lock(&context->m_task_mutex);
			for(i = 0; i < num_tasks; ++i){
				p = context->m_task_queue.front();
				context->m_task_queue.pop();
				p.first(p.second);
			}
			pthread_mutex_unlock(&context->m_task_mutex);
		}else{
			break;
		}
	}
}

bool UnixWorkerImplementation::Initialize(int id){
	m_task_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
	m_start_mutex = PTHREAD_MUTEX_INITIALIZER;
	m_start_cond = PTHREAD_COND_INITIALIZER;
	m_task_mutex = PTHREAD_MUTEX_INITIALIZER;
	m_task_cond = PTHREAD_COND_INITIALIZER;
	m_thread_args.first = this;
	m_thread_args.second = id;

	//Wait for the thread to "warm up".
	pthread_mutex_lock(&m_start_mutex);
	if(pthread_create(
		&m_thread,
		NULL,
		&UnixWorkerImplementation::AcceptTasks,
		&m_thread_args
	)){
		return false;
	}
	while(!m_is_started){
		pthread_cond_wait(&m_start_cond, &m_start_mutex);
	}
	pthread_mutex_unlock(&m_start_mutex);
	return true;
}

bool UnixWorkerImplementation::Release(){
	Kill();
	if(pthread_join(m_thread, NULL)){
		return false;
	}
	return true;
}

void UnixWorkerImplementation::SubmitTask(const Task &task, const string &args){
	pthread_mutex_lock(&m_task_mutex);
	m_task_queue.push(pair<Task, string>(task, args));
	pthread_cond_signal(&m_task_cond);
	pthread_mutex_unlock(&m_task_mutex);
}

void UnixWorkerImplementation::Kill(){
	pthread_mutex_lock(&m_task_mutex);
	m_is_running = false;
	pthread_cond_signal(&m_task_cond);
	pthread_mutex_unlock(&m_task_mutex);
}
	
