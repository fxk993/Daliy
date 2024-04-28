#include<iostream>
#include<pthread.h>
#include<queue>
#define MINSIZE 6;
#define MAXSIZE 20;
using namespace std;
typedef struct event
{
	void* args;
	void(*fun)(void*);
}Task;
class m_threadpool
{
public:
	m_threadpool(int livenumber,int busynumber);
	~m_threadpool() {};
public:
	void m_threadpool_init(m_threadpool* threadpool);
	bool m_threadpool_close(m_threadpool* pool);
	int get_tasknumber(m_threadpool*pool);
	bool add_task(m_threadpool* pool);
	
public:
	queue<Task> QTask;
	
	pthread_mutex_t mutex_task;
	pthread_mutex_t mutex_pool;
	pthread_cond_t task_empty;
	pthread_cond_t task_full;

	int shotdown = 0;
public:
	pthread_t managerid;
	pthread_t* workerid;
	const  int maxnum = MAXSIZE;
    const  int minnum =MINSIZE;
	int livenumber;
	int busynumber;
	int destroynumber;
	
};
void* worker(void* args);
void* manager(void* args);