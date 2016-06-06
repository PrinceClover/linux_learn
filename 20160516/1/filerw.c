#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

typedef struct{
	int num;
	char name[20];
	float score;
}stu;

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		printf("error args!\n");
		return -1;
	}
	stu s[3] = {{1,"reimo",88.8},{2,"morisa",77.7},{3,"sakuya",99.9}};
	stu s2[3];		
	int fd = open(argv[1],O_RDWR|O_CREAT);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	int ret = write(fd,s,sizeof(s));
	if(ret == -1)
	{
		perror("write");
		return -1;
	}
	ret = lseek(fd,0,SEEK_SET);
	if(ret == -1)
	{
		perror("lessk");
		return -1;
	}
	ret = read(fd,s2,sizeof(s2));
	if(ret == -1)
	{
		perror("open2");
		return -1;
	}
	ret = write(1,s2,sizeof(s2));
	if(ret == -1)
	{
		perror("write2");
		return -1;
	}
	printf("\n");
	int i;
	for(i = 0;i < 3;i++)
	{
		printf("%d\t%s\t%f\t\n",s2[i].num,s2[i].name,s2[i].score);
	}
	return 0;
}











