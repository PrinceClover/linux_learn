#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(void)
{
	pid_t pt,pt_w;
	pt = fork();
	if(pt == 0)
	{
		printf("child's pid:%d\n",getpid());
		printf("child has closed\n");
		exit(3);
	}
	else
	{
		int status;
		wait(&status);
		printf("child's pid:%d\nfather's pid:%d\n",pt,getpid());
		if(WIFEXITED(status))
		{
			printf("child exit:%d\n",WEXITSTATUS(status));
		}
		else
		{
			printf("child abord!\n");
		}
		exit(0);
	}
	return 0;
}
