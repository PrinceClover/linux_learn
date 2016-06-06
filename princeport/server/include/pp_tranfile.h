/*
 *name:pp_tranfile  // send/recv file headfile(server)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#ifndef __PP_TRANFILE_H__
#define __PP_TRANFILE_H__
#include "pp_head.h"

#define MAX_BUF 1024		//max size of buffer
#define RECV_NAME_LEN 64	//max length of username
#define RECV_PWD_LEN 256	//max length of password
#define TR_MAX_PATH 256		//max length of path

/*struct*/
typedef struct
{
	int len;			//buffer length
	char buf[MAX_BUF];	//file buffer
}data,*pdata;			//send/recv data package 
//samll train protocol ^_^

typedef struct
{
	char username[RECV_NAME_LEN];	//user name
	char password[RECV_PWD_LEN];	//password
}user,*puser;						//user information struct

void sendN(int,char*,int);		//send data package
void recvN(int,char*,int);		//recv data package
void sendFile(char*,char*,int);		//sendfile to client
void recvFile(char*,char*,int);		//recvfile from client

//TODO recv user information
void recvUserInfo(int,puser);//recv user information from cilent

#endif 
