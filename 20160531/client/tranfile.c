#include "tranfile.h"

void send_n(int fd,char *buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = send(fd,buf+total,len-total,0);
		total += ret;
	}
}

void recv_n(int fd,char *buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = recv(fd,buf+total,len-total,0);
		total += ret;
	}
}

void send_file(int fdw)
{
	data sbuf;
	memset(&sbuf,0,sizeof(sbuf));
	//send filename to client
	sbuf.len = strlen(FILENAME);
	strcpy(sbuf.buf,FILENAME);
	int ret = send(fdw,&sbuf,sbuf.len+sizeof(sbuf.len),0);
	if(-1 == ret)
	{
		perror("send");
		exit(-1);
	}
	//send file main body
	int fd = open(FILENAME,O_RDONLY);
	if(-1 == ret)
	{
		perror("open");
		exit(-1);
	}
	while(memset(&sbuf,0,sizeof(sbuf)),(sbuf.len = read(fd,sbuf.buf,sizeof(sbuf.buf))))
	{
		send_n(fdw,&sbuf,sbuf.len+sizeof(sbuf.len));
	}
	//send end of file
	ret = 0;
	send_n(fdw,&ret,sizeof(ret));
	close(fd);
	close(fdw);
}


