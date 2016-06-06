#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main(void)
{
	if(fork()>0)
	{
		exit(0);
	}
	setsid();
	chdir("/tmp");
	umask(0);
	int i;
	for(i=0;i<3;i++)
	{
		close(i);
	}
	int fd = open("/tmp/log",O_WRONLY|O_CREAT|O_EXCL);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	int ret;
	char buf[128]="I have run 5s\n";
	while(1)
	{
		ret = write(fd,buf,strlen(buf));
		if(-1 == ret)
		{
			perror("write");
			return -1;
		}
		sleep(5);
	}
	return 0;
}
