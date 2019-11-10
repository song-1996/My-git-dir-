#include"Selectsever.hpp"
int main(int argc ,char* argv[])
{
	if(argc!=2)
	{
		cerr<<"Useage:"<<argv[0]<<" port"<<endl;
		exit(1);
	}
	Selectserver* ps=new Selectserver(atoi(argv[1]));
	ps->InitServer();
	ps->Run();
	delete ps;
	return 0;
}

