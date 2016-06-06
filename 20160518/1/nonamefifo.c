#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(void)
{
	int fds[2];
	pipe(fds);
	char buf[16];
	if(fork()==0)
	{
		close(fds[0]);
		write(fds[1],"Hello",6);
		close(fds[1]);
		exit(0);
	}
	else
	{
		close(fds[1]);
		wait(NULL);
		if(read(fds[0],buf,sizeof(buf))>0)
		{
			printf("%s\n",buf);
		}
		close(fds[0]);
		exit(0);
	}
	return 0;
}
