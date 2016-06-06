#include"head.h"

void send_n(int fd,char *buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = send(fd,buf + total,len - total,0);
		total += ret;
	}
}

void recv_n(int fd,char *buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = recv(fd,buf + total,len - total,0);
		total += ret;
	}
}


