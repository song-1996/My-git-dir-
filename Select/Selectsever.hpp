#pragma once 
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/select.h>
using namespace std;
#define NUM sizeof(fd_set)*8
class Selectserver
{
private:
	int port;
	int lsock;
public:
	Selectserver(int _port=9090)
		:port(_port)
	{
		lsock=-1;
	}
	void InitServer()
	{
		lsock=socket(AF_INET,SOCK_STREAM,0);
		if(lsock<0)
		{
			cerr<<"create sock fail"<<endl;
			exit(2);
		}
		struct sockaddr_in local;
		local.sin_family=AF_INET;
		local.sin_port=htons(port);
		local.sin_addr.s_addr=htonl(INADDR_ANY);
		if((bind(lsock,(struct sockaddr*)&local,sizeof(local)))<0)
		{
			cerr<<"bind fail"<<endl;
			exit(3);
		}
		if(listen(lsock,5)<0)
		{
			cerr<<"listen fail"<<endl;
			exit(4);
		}
		int opt=1;
		setsockopt(lsock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	}
	void Run()
	{
		int array_fd[NUM];
		for(int i=0;i<NUM;i++)
			array_fd[i]=-1;
		array_fd[0]=lsock;
		int max=lsock;
		fd_set rfds;
		int sock=-1;
		for(;;)
		{
			FD_ZERO(&rfds);
			for(int i=0;i<NUM;i++)
			{
				if(array_fd[i]!=-1)
				{	
					FD_SET(array_fd[i],&rfds);
					if(max<array_fd[i])
						max=array_fd[i];
				}
			}
			struct sockaddr_in peer;
			socklen_t len=sizeof(peer);
			struct timeval timeout={1,0};
			int sval = select(max+1,&rfds,NULL,NULL,/*&timeout*/NULL);
			switch(sval)
			{
				case 0:
					cerr<<"select timeout"<<endl;
					break;
				case -1:
					cerr<<"select error"<<endl;
					break;
				default:
					for(int i=0;i<NUM;i++)
					{
						if(array_fd[i]!=-1)
						{
							if(FD_ISSET(array_fd[i],&rfds))
							{
								if(array_fd[i]==lsock)
								{
									sock = accept(lsock,(struct sockaddr*)&peer,&len);
									if(sock<0)
									{		
										cerr<<"accept error"<<endl;
										continue;
									}	
									else
									{
									 cout<<"get a new client..."<<endl;
										int j=1;
										for(;j<NUM;j++)
										{
											if(array_fd[j]==-1)
											{	
												array_fd[j]=sock;
												break;
											}
										}
										if(j>=NUM)
										{
											cerr<<"NUM fall"<<endl;
											close(sock);
										}
								   	}	
								}
								else
								{
									char buf[10240];
									ssize_t s = recv(array_fd[i],buf,sizeof(buf)-1,0);
									if(s>0)
									{
										buf[s]=0;
										cout<<"Client# "<<buf<<endl;
									}
									else if(s==0)
									{
										cerr<<"Client quit..."<<endl;
										close(array_fd[i]);
										array_fd[i]=-1;
									}
									else 
									{
										cerr<<"Read error..."<<endl;
										close(array_fd[i]);
										array_fd[i]=-1;
									}
								}
							}
						}
					
					}
					break;
			}
			
		}
	}
	~Selectserver()
	{
		if(lsock>=0)
			close(lsock);
	}
};

