#include"head.h"

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("error args\n");
		return -1;
	}
	int fd = open(argv[1],O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	struct stat s;
	fstat(fd,&s);
	char *p;
	p = (char*)mmap(NULL,s.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if((char*)-1==p)
	{
		perror("mmap");
		return -1;
	}
	int ret = write(1,p,6);
	if(-1 == ret)
	{
		perror("write1");
		return -1;
	}
	sprintf(p,"%s","world\n");
	ret = write(1,p,6);
	if(-1 == ret)
	{
		perror("write2");
		return -1;
	}
	ret = munmap(p,s.st_size);
	if(ret != 0)
	{
		perror("munmap");
		return -1;
	}
	return 0;
}
