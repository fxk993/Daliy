#ifndef THREDAPOOL_H
#define THREDAPOOL_H
#include<list>
#include<pthread.h>
#include<exception>
#include"lock.h"
using namespace std;
template<class T>
class threadpool
{
public:
	threadpool(int actor_model,connection_pool*connpool,int number,int max);
	~threadpool();
public:
	append(T* request, int state);
	append_p(T* request);
	static void* worker(void* args);
	void run();
	

private:
	int m_thread_number;
	int m_max_request;
	pthread_t* m_worker;
	pthread_t m_manager;
	list<T*>m_workqueue;
	locker m_queuelocker;
	sem m_queuesem;
	connection_pool* m_connPool;
	int actor_model;
};


template <class T>
threadpool<T>::threadpool(int actor_model, connection_pool* connPool, int number, int max)
{
	if (thread_number < 0 || max <= 0)
	{
		throw exception();
	}
	m_worker = new pthread_t[m_thread_number];
	if (!m_worker)
	{
		throw exception();
	}
	for (int i = 0; i < m_thread_number; i++)
	{
		if (pthread_create(&m_worker[i], nullptr, worker, this) != 0)
		{
			delete[] m_worker;
			throw exception();
		}
		if (pthread_detach(&m_worker[i]) != 0)
		{
			delete[] m_worker;
			throw exception();
		}



	}
}
threadpool<T>::~threadpool()
{
	delete[] m_worker;
}
void threadpool<T>::worker(void*args)
{
	threadpool* pool = (threadpool*)args;
	pool->run();
}
void threadpool<T>::run()
{
	while (true)
	{
		m_queuesem.wait();
		m_queuelocker.lock();
		if (m_workqueue.empty())
		{
			m_queuelocker.unlock();
			continue;
		}
		T* request = m_workqueue.front();
		m_workqueue.pop_front();
		m_queuelocker.unlock();
		if (!request)
		{
			continue;
		}
		if (actor_model == 1)
		{
			if (request->state == 0)
			{
				if (request->read_once())
				{
					request->improv = 1;
					request->process();
				}
				else
				{
					request->improv = 1;
					request->timer_flag = 1;
				}
			}
		}
		else
		{
			request->process();
		}
	}
}

#endif
