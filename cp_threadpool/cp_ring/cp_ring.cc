#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<vector>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
class Ringqueue
{
private:
	vector<int> q;
	int cap;
	sem_t data_sem;
	sem_t space_sem;
	int p_step;
	int c_step;
public:
	Ringqueue(int _cap=4)
		:cap(_cap)
		 ,q(_cap)
	{
		sem_init(&data_sem,0,0);
		sem_init(&space_sem,0,cap);
		p_step=0;
		c_step=0;
	}	
	void PushData(const int data)
	{
		sem_wait(&space_sem);
		q[p_step]=data;
		p_step++;
		p_step %= cap;
		sem_post(&data_sem);
	}
	void PopData(int& out)
	{
		sem_wait(&data_sem);
		out=q[c_step];
		c_step++;
		c_step %= cap;
		sem_post(&space_sem);
	}
	~Ringqueue()
	{
		sem_destroy(&data_sem);
		sem_destroy(&space_sem);
	}
};

void* producter(void* arg)
{
	Ringqueue* pr=(Ringqueue*)arg;
	while(1)
	{
		int data=rand()%100 + 1;
		pr->PushData(data);
		cout<<"producter data is:"<<data<<endl;
		sleep(2);
	}
}
void* consumer(void* arg)
{
	
	Ringqueue* pr=(Ringqueue*)arg;
	while(1)
	{
		int out;
		pr->PopData(out);
		cout<<"consumer data is:" << out<<endl;
	}
}
int main()
{
	srand((unsigned long)time(NULL));
	Ringqueue* pr=new Ringqueue();
	pthread_t c,p;
	pthread_create(&p,NULL,producter,(void*)pr );	
	pthread_create(&c,NULL,consumer,(void*)pr );
	pthread_join(p,NULL);
	pthread_join(c,NULL);
}
