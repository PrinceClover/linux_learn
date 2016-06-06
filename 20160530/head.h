#ifndef __HEAD_H__
#define __HEAD_H__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>

#define FILENAME "proud.mp4"	//文件名
#define BUFSIZE 1024			//缓冲区大小

#define BUSY 1				//子进程状态 忙碌
#define FREE 0				//子进程状态 空闲

typedef struct				//子进程信息结构体
{
	pid_t pid;				//子进程id
	int fds;				//管道端口号
	int status;				//状态BUSY或者FREE
}child,*pchild;

typedef struct				//数据包结构体
{
	int len;				//缓冲区buf的长度
	char buf[BUFSIZE];		//缓冲区
}data,*pdata;

#endif
