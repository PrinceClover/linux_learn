/*
 *name:pp_system.h  //system headfile,to initialize system
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#ifndef __PP_SYSTEM_H__
#define __PP_SYSTEM_H__
#include "pp_head.h"
#include "pp_order.h"
#include "pp_tranfile.h"
#include "pp_tpool.h"

#define CONF_FILE_NAME "../conf/sconf.config"  //config filepath
#define PATH_LEN 256		//max length of path
#define CONF_SIZE 1024		//max size of struct config 
#define CONF_NUM 3			//number of configs
#define MAX_CONN 5			//max number of connections(threads),at most 128

//pointer of function type
typedef void* (*pfunc)(void*);

/*struct*/
typedef struct
{
	char ip[30];			//server ip address
	char port[6];			//server port
	char filepath[PATH_LEN];//root path of sendfile
}config,*pconfig;			//config struct

typedef struct
{
	pthread_t *thread;		//thread pool array
	int th_num;				//total number of threads
	pthread_cond_t cond;	//conditional variate
	int capacity;			//max number of nodes in queue
	Que que;				//queue of client fd
	pfunc child_fun;		//child handle function
	char rootpath[PATH_LEN];//path of data(from config file)
	int flag;				//initialize factory flag;
	int conn_num;			//connection number
}factory,*pfact;			//factory struct

/*function*/

void loadConf(char*,pconfig);		//load config file
void startUpServer(int*,pconfig);	//startup server and listening
void factoryInit(pfact,pconfig,int,int,pfunc);	//initialize factory
void factoryStart(pfact);						//start factory
void* childHandle(void*);	//thread handle function

#endif
