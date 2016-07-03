#include "worker.h"

Worker::Worker(){
}

Worker::~Worker(){
}

bool Worker::Initialize(int id){
#ifdef _WIN32
#elif MACOS
#else
	if(!(m_impl = new UnixWorkerImplementation())){
		return false;
	}
#endif
	if(!(m_impl->Initialize(id))){
		return false;
	}
	return true;
}

bool Worker::Release(){
	if(m_impl){
		if(!(m_impl->Release())){
			delete m_impl;
			return false;
		}
		delete m_impl;
	}
	return true;
}

void Worker::SubmitTask(const Task &task, const string &args){
	m_impl->SubmitTask(task, args);	
}

