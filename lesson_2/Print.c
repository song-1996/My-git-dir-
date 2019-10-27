#include<stdio.h>
#include<string.h>
int main()
{

	const char* msg="hello world\n";
	printf("%s",msg);
	fwrite(msg,strlen(msg),1,stdout);
	fprintf(stdout,"%s",msg);
	return 0;
}

