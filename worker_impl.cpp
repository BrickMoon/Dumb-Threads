#include "worker_impl.h"

WorkerImplementation::WorkerImplementation(){
}

WorkerImplementation::~WorkerImplementation(){
}

Task::Task(){
}

Task::Task(function<void(const string &)> task){		
	m_task = task;
}

Task::~Task(){

}

void Task::operator()(const string &args){
	m_task(args);
}
