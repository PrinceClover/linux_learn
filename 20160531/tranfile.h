#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"

#define MAXSIZE 1024    	 	//maxsize of buffer
#define FILENAME "proud.mp4"	//filename to send/recv

//send/recv buffer struct
typedef struct
{
	int len;
	char buf[MAXSIZE];
}data,*pdata;

//send/recv file function
void send_file(int fdw);	//sendfile to client
void recv_file(int fdr);	//recvfile from server
void send_n(int fd,char* buf,int len);	//send file mainbody
void recv_n(int fd,char* buf,int len);	//recv file mainbody
#endif

