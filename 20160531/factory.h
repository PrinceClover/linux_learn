#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "que.h"
#include "tranfile.h"

//pointer of function type
typedef void* (*pfunc)(void*);

/*factory struct*/
//factory struct
typedef struct
{
	pthread_t *thread;		//thread pool array
	int th_num;				//total number of threads
	pthread_cond_t cond;	//conditional variate
	int capacity;			//max number of nodes in queue
	Que que;				//queue of client fd
	pfunc child_fun;		//pointer of child_function
	int flag;				//initialize factory flag
}factory,*pfact;

/*factory handle function*/
//initialize factory
void factory_init(pfact,int,int,pfunc);

//start factory
void factory_start(pfact);

//thread working function
void* child_thread(void*);

#endif
