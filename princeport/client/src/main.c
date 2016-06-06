/*
 *name:main.c  // start clinet from here
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_system.h"

int main(void)
{
	//loading configfile
	config conf;
	memset(&conf,0,sizeof(conf));
	loadConf(CONF_FILE_NAME,&conf);
//	printf("ip:%s\nport:%s\nfilepath:%s\n",conf.ip,conf.port,conf.filepath);
	//connect server
	int cfd;		//client socket
	connServer(&cfd,&conf);
	printf("server has connected!\n");
//	printf("cfd:%d\n",cfd);
	//user handle
	order od;	//order
	int ret;	//receive return value
	while(1)
	{
		//read order form stdin
		memset(&od,0,sizeof(od));
//		gets(od.info);
		printf("prince@client>");
		fflush(stdout);
		ret = read(0,od.info,sizeof(od.info));
		if(-1 == ret)
		{
			perror("read");
			return -1;
		}
		else if(0 == ret)
		{
			continue;
		}
		//send order to server
		sendOrder(cfd,&od);
//		printf("order = %d\ninfolen = %d\ninfo = %s\n",od.order,od.infolen,od.info);
		handleOrder(&od,conf.filepath,cfd);
//		printf("send success\n");
	}
	return 0;
}
