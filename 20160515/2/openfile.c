#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("error args!\n");
		return -1;
	}
	int fd = open(argv[1],O_RDWR|O_CREAT,0666);
	if(fd < 0)
	{
		perror("open1");
		return -1;
	}
	printf("file opened\nfd = %d\n",fd);
	char buf[128] = "hello";
	int ret = write(fd,buf,strlen(buf));
	if(ret < 0)
	{
		perror("write");
		return -1;
	}
	ret = close(fd);
	if(ret < 0)
	{
		perror("close");
		return -1;
	}
	printf("flie closed\n");
	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open2");
		return -1;
	}
	printf("file opened\nfd = %d\n",fd);
	ret = read(fd,buf,sizeof(buf));
	if(ret < 0)
	{
		perror("read");
		return -1;
	}
	printf("%s\n",buf);
	return 0;
}
