/*
 *name:main.c  // start server from here
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_system.h"

int main(void)
{
	//loading configfile
	config conf;	//config data
	memset(&conf,0,sizeof(conf));
	loadConf(CONF_FILE_NAME,&conf);
//	printf("%s - %s - %s\n",conf.ip,conf.port,conf.filepath);
	//startup thread
	factory fact;	//factory variate
	memset(&fact,0,sizeof(fact));
	factoryInit(&fact,&conf,MAX_CONN,MAX_QUE,childHandle);
	factoryStart(&fact);
	//startup server
	int sfd;		//server socket
	startUpServer(&sfd,&conf);
	printf("the Server has startup!\nlistening...\n");
	//handle accept from client
	Node rfn;		//client information accept from client
	while(1)
	{
		rfn.rfd = accept(sfd,NULL,NULL);
		if(-1 == rfn.rfd)
		{
			perror("accept");
			return -1;
		}
		/*@TODO recv user information and init rfn*/
		strcpy(rfn.path,conf.filepath);
		queEnque(&fact.que,&rfn);
		pthread_cond_signal(&fact.cond);
	}
	return 0;
}
