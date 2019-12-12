#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include"Util.hpp"

using namespace std;
int GetData(const string& str)
{
	size_t pos=str.rfind('=');
	if(string::npos!=pos)
	{
		string sub=str.substr(pos+1);
		return Util::StringToInt(sub);
	}
}
int main()
{

	string args;
	string method = getenv("METHOD");
	if(method=="GET")
	{
		args = getenv("QUERY_STRING");
	}
	else if(method=="POST")
	{
		string cl=getenv("CONTENTLENGTH_LENGTH");
		int content_length=Util::StringToInt(cl);
		char c;
		while(content_length--)
		{
			read(0,&c,1);
			args.push_back(c);
		}
	}
	else
	{
		//bug?
	}
	//data1=100 & data2=200 
	size_t pos=args.find('&');
	if(string::npos!=pos)
	{
		string str1=args.substr(0,pos);
		string str2=args.substr(pos+1);
		
		int data1=GetData(str1);
		int data2=GetData(str2);

		cout<<data1<<"+"<<data2<<"="<<data1+data2<<endl;
		cout<<data1<<"-"<<data2<<"="<<data1-data2<<endl;
		cout<<data1<<"*"<<data2<<"="<<data1*data2<<endl;
		cout<<data1<<"/"<<data2<<"="<<data1/data2<<endl;
		cout<<endl;
	}
	return 0;
}

