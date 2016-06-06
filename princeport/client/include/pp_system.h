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

#define CONF_FILE_NAME "../conf/sconf.config"  //config filepath
#define PATH_LEN 256		//max length of path
#define CONF_SIZE 1024		//max size of struct config 
#define CONF_NUM 3			//number of configs

/*struct*/
typedef struct
{
	char ip[30];			//server ip address
	char port[6];			//server port
	char filepath[PATH_LEN];//root path of sendfile
}config,*pconfig;			//config struct

/*function*/

void loadConf(char*,pconfig);		//load config file
void connServer(int*,pconfig);		//connect server
 
#endif
