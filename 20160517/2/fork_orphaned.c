#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	pid_t pt;
	pt = fork();
	if(pt == 0)
	{
		printf("child pid:%d\n",getpid());
		while(1);
	}
	else
	{
		printf("father's pid:%d\n",getpid());
		printf("father has closed\n");
		exit(0);
	}
	return 0;
}
