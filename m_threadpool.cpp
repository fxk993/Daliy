#include<iostream>
#include<pthread.h>

#include "m_threadpool.h"
#include<deque>
#include<string.h>
m_threadpool::m_threadpool(int livenumber=0, int busynumber=0) :livenumber(livenumber),
busynumber(busynumber)
{}

void m_threadpool::m_threadpool_init(m_threadpool* threadpool)
{
	if (pthread_mutex_init(&mutex_pool, nullptr) && pthread_mutex_init(&mutex_pool, nullptr) && pthread_cond_init(&task_empty, nullptr) && pthread_cond_init(&task_full, nullptr))
		cout << "mutex error!" << endl;
	workerid = new pthread_t[maxnum];
	pthread_create(&managerid, nullptr, manager, &threadpool);
	memset(workerid, 0, sizeof(workerid));
	for (int i = 0; i < minnum; i++)
	{
		pthread_create(&workerid[i], nullptr, worker, &threadpool);
		pthread_detach(workerid[i]);
		threadpool->livenumber++;
	}
}

int m_threadpool::get_tasknumber(m_threadpool* pool)
{
	pthread_mutex_lock(&mutex_pool);
	int x = pool->QTask.size();
	pthread_mutex_unlock(&mutex_pool);
	return x;
}

bool m_threadpool::m_threadpool_close(m_threadpool*pool)
{
	pool->shotdown = 1;
	pthread_mutex_destroy(&mutex_pool);
	pthread_mutex_destroy(&mutex_task);
	pthread_cond_destroy(&task_empty);
	pthread_cond_destroy(&task_full);
	int** ret = nullptr;
	pthread_join(pool->managerid,(void**)ret);
	if (**ret == 1)
		return true;

	return false;
}
void func1(void* args)
{
	int* i = (int*)args;
	cout << "task1 has finished" << endl;
}

bool m_threadpool::add_task(m_threadpool* pool)
{

	for (int i = 0; i <= 100; i++)
	{
		int x = i;
		Task t;
		t.args = &x;
		t.fun = func1;
		pool->QTask.push(t);
	}
	return false;
}
void* worker(void* args) {
	m_threadpool* pool = (m_threadpool*)args;
	while (1)
	{	
		if (pool->shotdown == 1)
		{
			break;
		}

		Task t;
		pthread_mutex_lock(&pool->mutex_task);
		if (pool->QTask.empty() == 1)
		{
			pthread_cond_wait(&pool->task_empty, &pool->mutex_task);
			if (pool->destroynumber > 0)
			{
				pthread_mutex_unlock(&pool->mutex_task);
				break;
			}
			t = pool->QTask.front();
			pool->QTask.pop();
			pool->busynumber++;
			pthread_mutex_unlock(&pool->mutex_task);
		}
		
		t.fun(t.args);
		pthread_mutex_lock(&pool->mutex_pool);
		pool->busynumber--;
		pthread_mutex_unlock(&pool->mutex_pool);
		
	}
}
void* manager(void* args)
{
	m_threadpool* pool = (m_threadpool*)args;
	
	while (1) {
		if (pool->shotdown == 1)
		{
			int n = 1;
			pthread_exit(&n);
		}
		else if (pool->livenumber < pool->QTask.size() && 2 * pool->livenumber>pool->QTask.size() && 2 * pool->livenumber < pool->maxnum)
		{
			pthread_mutex_lock(&pool->mutex_pool);
			int ans = pool->livenumber;
			pthread_mutex_unlock(&pool->mutex_pool);
			for (int i = pool->livenumber - 1; i < 2 * ans - 1; i++)
			{
				pthread_create(&pool->workerid[i], nullptr, worker, pool);
				pthread_detach(pool->workerid[i]);
				pool->livenumber++;
			}
		}
		else if (pool->livenumber > 2 * pool->QTask.size() && pool->livenumber - 2 > pool->minnum)
		{
			pool->destroynumber = 2;
			for (int i = 0; i < 2; i++)
			{
				pthread_cond_signal(&pool->task_empty);
				pool->destroynumber--;
			}
		}
	}
}