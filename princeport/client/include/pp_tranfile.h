/*
 *name:pp_tranfile  // send/recv file headfile(client)
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

/*struct*/
typedef struct
{
	int len;			//buffer length
	char buf[MAX_BUF];	//file buffer
}data,*pdata;			//send/recv file struct
//small train protocol ^_^

typedef struct
{
	char username[RECV_NAME_LEN];	//user name
	char password[RECV_PWD_LEN];	//password
}user,*puser;						//user information struct

void sendN(int,char*,int);		//send file mainbody
void recvN(int,char*,int);		//recv file mainbody
void sendFile(int,char*,char*);	//sendfile to server
void recvFile(int,char*,char*);	//recvfile from server

//TODO send user information
void sendUserInfo(int,puser);//recv user information to server

#endif 
