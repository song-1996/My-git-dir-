#include<iostream>
#include<queue>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
class Task
{
	private:
		int x;
		int y;
	public:
		Task(int _x=0,int _y=0)
			:x(_x)
			 ,y(_y)
		{}
		int Run()
		{
			return x+y;	
		}
		~Task()
		{
		}
};
class Threadpool
{
	private:
		int num;
		queue<Task> q;
		pthread_mutex_t lock;
		pthread_cond_t cond;
	public:
		void Lockqueue()
		{
			pthread_mutex_lock(&lock);
		}
		void Unlockqueue()
		{
			pthread_mutex_unlock(&lock);
		}
		bool Isqueueempty()
		{
			return q.size()==0;
		}
		void Threadwait()
		{
			pthread_cond_wait(&cond,&lock);
		}
		void Notitythread()
		{

			pthread_cond_signal(&cond);
		}
		void PushTask(Task& t)
		{	
			q.push(t);
		}	
		void PopTask(Task& t)
		{
			t = q.front();
			q.pop();
		}	
	public:
		Threadpool(int _num=5)
			:num(_num)
		{	
			pthread_mutex_init(&lock,NULL);
			pthread_cond_init(&cond,NULL);
		}
		static void* Runtask(void* arg)
		{
			pthread_detach(pthread_self());
			Threadpool* pt=(Threadpool*)arg;
			for(;;)
			{
				pt->Lockqueue();
				if(pt->Isqueueempty())
				{
					pt->Threadwait();
				}
				Task t;
				pt->PopTask(t);
				pt->Unlockqueue();
				int result=t.Run();
				cout<<"Thread: "<<pthread_self()<<",result: "<<result<<endl;
			}
		}
		void InitThreadpool()
		{
			int i=0;
			pthread_t tid;
			for(;i<num;i++)
			{
				pthread_create(&tid,NULL,Runtask,(void*)this);
			}
		}
		void ADDTask(Task& t)
		{
			Lockqueue();
			PushTask(t);
			Unlockqueue();
			Notitythread();
		}
		~Threadpool()
		{
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&cond);
		}

};
int main()
{	
	srand((unsigned long)time(NULL));
	Threadpool* pt=new Threadpool();
	pt->InitThreadpool();
	for(;;)
	{
		int	x=rand()%100 +1;
		int y=rand()%100 +1;
		Task t(x,y);
		pt->ADDTask(t);
		sleep(1);
	}
	delete pt;
	return 0;
}
