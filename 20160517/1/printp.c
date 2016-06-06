#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>

int main(void)
{
	//pid_t pt;
	printf("pid:%d\tppid:%d\nuid:%d\tgid:%d\neuid:%d\tegid:%d\n",getpid(),getppid(),getuid(),getgid(),geteuid(),getegid());
	return 0;
}
