#include "thread_pool.h"

#ifdef _WIN32
#elif MACOS
#else
#include <unistd.h>
#endif

ThreadPool::ThreadPool(){
	m_workers = NULL;
}

ThreadPool::~ThreadPool(){
}

bool ThreadPool::Initialize(){
//***UNIX ONLY - NOT PORTABLE ***
#ifdef _WIN32
#else
	m_num_threads = sysconf(_SC_NPROCESSORS_ONLN) - 1;
#endif
//*** ======================  ***		
	
	if(m_num_threads < 2){
		return false;
	}
	if(!(m_workers = new Worker[m_num_threads])){
		return false;
	}
	for(int i = 0; i < m_num_threads; ++i){
		if(!(m_workers[i].Initialize(i))){
			return false;
		}
	}
	return true;
}

bool ThreadPool::Initialize(unsigned num_threads){
}

bool ThreadPool::Release(){
	if(m_workers){
		for(int i = 0; i < m_num_threads; ++i){
			if(!m_workers[i].Release()){
				delete [] m_workers;
				return false;
			}
		}
		delete [] m_workers;
	}
	return true;
}

void ThreadPool::SubmitTask(const Task &task, const string &args){
	static unsigned which = 0;
	m_workers[which].SubmitTask(task, args);
	which = (which + 1) % m_num_threads;
}

