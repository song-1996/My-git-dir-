#include<iostream>
#include<pthread.h>
#include<queue>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
using namespace std;


class BlockQueue
{
private:
	queue<int> q;
	int cap;
	pthread_mutex_t lock;
	pthread_cond_t full;
	pthread_cond_t empty;
private:
	void QueueLock()
	{
		pthread_mutex_lock(&lock);
	}
	void QueueUnlock()
	{
		pthread_mutex_unlock(&lock);
	}
	void SignalConsumer()
	{
		pthread_cond_signal(&empty);
	}
	void SignalProducter()
	{
		pthread_cond_signal(&full);
	}
	void ConsumerWait()
	{
		pthread_cond_wait(&empty,&lock);
	}
	void ProducterWait()
	{
		pthread_cond_wait(&full,&lock);
	}
	bool QueueFull()
	{
		return q.size() == cap;
	}
	bool QueueEmpty()
	{
		return q.size() == 0;
	}

public:
	BlockQueue(int _cap=8)
		:cap(_cap)
	{
		pthread_mutex_init(&lock,NULL);
		pthread_cond_init(&empty,NULL);
		pthread_cond_init(&full,NULL);
	}
	void PushData(const int data)
	{
		QueueLock();
		if(QueueFull())
		{
			cout<<"queue is full..."<<endl;
			SignalConsumer();
			ProducterWait();
		}
		q.push(data);
		QueueUnlock();
	}
	void PopData(int& out)
	{
		QueueLock();
		if(QueueEmpty())
		{
			cout<<"queue is empty..."<<endl;
			SignalProducter();
			ConsumerWait();
		}
		out=q.front();
		q.pop();
		QueueUnlock();
	}
	~BlockQueue()
	{
		pthread_mutex_destroy(&lock);
		pthread_cond_destroy(&full);
		pthread_cond_destroy(&empty);
	}
};
void* producter(void* arg)
{
	BlockQueue* pb=(BlockQueue*)arg;
	while(1)
	{
		int data=rand()%100 + 1; 
		pb->PushData(data);
		cout<<"prodct data is:" <<data<<endl;
		sleep(1);
	}
}
void* consumer(void* arg)
{	
	int out;
	BlockQueue* pb=(BlockQueue*)arg;
	while(1)
	{
		pb->PopData(out);
		cout<<"consumer data is:" <<out<<endl;
	}
}
int main()
{
		srand((unsigned long)time(NULL));
		BlockQueue* pb=new BlockQueue(4);
		pthread_t p,c;
		pthread_create(&p,NULL,producter,(void*)pb);
		pthread_create(&c,NULL,consumer,(void*)pb);
		pthread_join(p,NULL);
		pthread_join(c,NULL);
		delete pb;
		return 0;
}
