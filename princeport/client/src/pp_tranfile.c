/*
 *name:pp_tranfile.c  // achieve function in tranfile(client)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_tranfile.h"

void sendFile(int fdw,char* path,char* filename)
{
	data sbuf;
	char buf[MAX_BUF];
	struct stat st;
	memset(buf,0,sizeof(buf));
	memset(&sbuf,0,sizeof(sbuf));
	memset(&st,0,sizeof(st));
	sprintf(buf,"%s%s%s",path,"/",filename);
	int ret = stat(buf,&st);
	if(-1 == ret)
	{
		sbuf.len = -1;
		sendN(fdw,(char*)&sbuf.len,sizeof(sbuf.len));
		printf("no such file name [%s]!\n",filename);
		return;
	}
	if(st.st_mode & S_IFDIR)
	{
		sbuf.len = -1;
		sendN(fdw,(char*)&sbuf.len,sizeof(sbuf.len));
		printf("[%s] is a dir!\n",filename);
		return;
	}
	int fd = open(buf,O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	sbuf.len = strlen(filename);
	strcpy(sbuf.buf,filename);
	printf("start update file [%s]\n",filename);
	sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
	while(memset(&sbuf,0,sizeof(sbuf)),(sbuf.len = read(fd,sbuf.buf,sizeof(sbuf.buf))))
	{
		sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
	}
	ret = 0;
	sendN(fdw,(char*)&ret,sizeof(ret));
	printf("file [%s] update complete!\n",filename);
	close(fd);
}

void recvFile(int fdr,char* path,char* filename)
{
	data rbuf;
	char buf[MAX_BUF];
	memset(buf,0,sizeof(buf));
	memset(&rbuf,0,sizeof(rbuf));
	int namelen = strlen(filename);
	recvN(fdr,(char*)&rbuf.len,sizeof(rbuf.len));
	if(namelen != rbuf.len)
	{
		recvN(fdr,rbuf.buf,rbuf.len);
		printf("%s\n",rbuf.buf);
		return;
	}
	printf("start downland file [%s]!\n",filename);
	recvN(fdr,rbuf.buf,rbuf.len);
	sprintf(buf,"%s%s%s",path,"/",filename);
	int fd = open(buf,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	while(1)
	{
		memset(&rbuf,0,sizeof(rbuf));
		recvN(fdr,(char*)&rbuf.len,sizeof(rbuf.len));
		if(rbuf.len > 0)
		{
			recvN(fdr,rbuf.buf,rbuf.len);
			write(fd,rbuf.buf,rbuf.len);
		}	
		else
		{
			break;
		}
	}
	close(fd);
	printf("file [%s] downland complete!\n",filename);
}

void sendN(int fd,char* buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = send(fd,buf+total,len-total,0);
		total += ret;
	}
}

void recvN(int fd,char* buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = recv(fd,buf+total,len-total,0);
		total += ret;
	}
}
