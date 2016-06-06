/*
 *name:pp_tranfile.c  // achieve function in tranfile(server)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_tranfile.h"

void recvFile(char* nowpath,char* filename,int fdr)
{
	data sbuf;
	char tpath[TR_MAX_PATH];
	memset(tpath,0,sizeof(tpath));
	memset(&sbuf,0,sizeof(sbuf));
	recvN(fdr,(char*)&sbuf.len,sizeof(sbuf.len));
	if(-1 == sbuf.len)
	{
		return;
	}
	recvN(fdr,sbuf.buf,sbuf.len);
	sprintf(tpath,"%s%s%s",nowpath,"/",filename);
	int fd = open(tpath,O_RDWR|O_CREAT,0666);
	if(-1 == fd)
	{
		perror("open");
		return;
	}
	while(1)
	{
		memset(&sbuf,0,sizeof(sbuf));
		recvN(fdr,(char*)&sbuf.len,sizeof(sbuf.len));
		if(sbuf.len > 0)
		{
			recvN(fdr,sbuf.buf,sbuf.len);
			write(fd,sbuf.buf,sbuf.len);
		}
		else
		{
			break;
		}
	}
	close(fd);
}

void sendFile(char* nowpath,char* filename,int fdw)
{
	data sbuf;
	char tpath[TR_MAX_PATH];
 	struct stat statbuf;	
	memset(tpath,0,sizeof(tpath));
	memset(&sbuf,0,sizeof(sbuf));
	memset(&statbuf,0,sizeof(statbuf));
	sprintf(tpath,"%s%s%s",nowpath,"/",filename);
	int ret = stat(tpath,&statbuf);
	if(-1 == ret)
	{
       	sprintf(sbuf.buf,"can not find file [%s]",filename);
		sbuf.len = strlen(sbuf.buf);
		sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
		return;
	}
	if(statbuf.st_mode & S_IFDIR)
	{
      	sprintf(sbuf.buf,"[%s] is a dir",filename);
		sbuf.len = strlen(sbuf.buf);
		sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
		return;
	}
	memset(&sbuf,0,sizeof(sbuf));
	sbuf.len = strlen(filename);
	strcpy(sbuf.buf,filename);
	sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
//	printf("tpath:%s\n",tpath);
	int fd = open(tpath,O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		exit(-1);
	}
	while(memset(&sbuf,0,sizeof(sbuf)),(sbuf.len = read(fd,sbuf.buf,sizeof(sbuf.buf))))
	{
		sendN(fdw,(char*)&sbuf,sbuf.len+sizeof(sbuf.len));
	}
	ret = 0;
	sendN(fdw,(char*)&ret,sizeof(ret));
	close(fd);
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

void recvN(int fd,char *buf,int len)
{
	int total = 0;
	int ret;
	while(total < len)
	{
		ret = recv(fd,buf+total,len-total,0);
		total += ret;
	}
}


