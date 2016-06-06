/*
 *name:pp_tpool.h  // threads pool headfile(server)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#ifndef __PP_TPOOL_H__
#define __PP_TPOOL_H__
#include "pp_head.h"

#define MAX_QUE 100		//max size of queue
#define NAME_LEN 64	//max size of username
#define PWD_LEN	128		//max size of password
#define USER_PATH_LEN 256 //max size of user rootpath

/*struct*/
typedef struct node
{
	int rfd;			//request fd from client
	char username[NAME_LEN];//client user name
	char password[PWD_LEN];//client user password
	char path[USER_PATH_LEN];//user root path
	struct node* next;	//point next node
}Node,*pNode;			//queue struct

typedef struct
{
	pNode phead,ptail;	//head and tail of queue
	pthread_mutex_t mutex;	//mutex of queue
	int size;			//size of queue
}Que,*pQue;

/*function*/
void queInit(pQue);			//initialize queue
void queEnque(pQue,pNode);  //enqueue
void queDeque(pQue,pNode);	//dequeue
char* pathJoint(char*,int);	//joint path with rootpath by order

#endif
