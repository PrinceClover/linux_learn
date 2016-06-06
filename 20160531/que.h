#ifndef __QUE_H__
#define __QUE_H__
#include "head.h"
/*queue data struct*/
//queue node struct
typedef struct node
{
	int rfd;			//request fd from client
	struct node* next;	//point next node
}Node,*pNode;

//queue srtuct
typedef struct
{
	pNode phead,ptail;		//head and tail of queue
	pthread_mutex_t mutex;	//mutex lock of queue
	int size;				//size of queue
}Que,*pQue;

/*queue handle function*/
//initialize queue
void que_init(pQue);

//enqueue
void que_enque(pQue,int);

//dequeue
void que_deque(pQue,int*);

#endif
