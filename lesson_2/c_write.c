#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE* fp=fopen("myfile","w");
	if(fp==NULL)
	{
		printf("fopen fail\n");
		exit(1);
	}
	const char* msg="hello world!\n";
	int count=10;
	while(count--)
		fwrite(msg,strlen(msg),1,fp);
	fclose(fp);
	return 0;
}

