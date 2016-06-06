/*
 *name:pp_system.c  // achieve function in pp_system(server)
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_system.h"

void* childHandle(void *pf)
{
	pfact p = (pfact)pf;
	Node rfn;
	char rootpath[PATH_LEN];
	memset(rootpath,0,sizeof(rootpath));
	memset(&rfn,0,sizeof(rfn));
	while(1)
	{
		//lock queue
		pthread_mutex_lock(&p->que.mutex);
		if(0 == p->que.size)
		{
			pthread_cond_wait(&p->cond,&p->que.mutex);
		}
		queDeque(&p->que,&rfn);	//enque rfd
		//unlock queue
		pthread_mutex_unlock(&p->que.mutex);
		(p->conn_num)++;
		printf("a client has connected! total:%d\n",p->conn_num);
		order od;	//order
		data sd;	//data package
		strcpy(rootpath,rfn.path);
		while(1)
		{
			memset(&od,0,sizeof(od));
			memset(&sd,0,sizeof(sd));
			recvOrder(rfn.rfd,&od);
//			printf("order = %d\ninfolen = %d\ninfo = %s\n",od.order,od.infolen,od.info);
			handleOrder(&od,rfn.path,rfn.rfd,rootpath);
			if(ORDER_EXIT == od.order)
			{
				break;
				printf("while break\n");
			}	
//			printf("path:%s\n",rfn.path);
		}
		(p->conn_num)--;
		printf("a client has disconnected! total:%d\n",p->conn_num);
	}
}
void loadConf(char* confpath,pconfig conf)
{
	int conflen = sizeof(*conf);
	int fd = open(confpath,O_RDONLY);
	if(-1 == fd)
	{
		perror("read");
		exit(-1);
	}
	char buf[CONF_SIZE];
	int ret = read(fd,buf,sizeof(buf));
	if(0 == ret)
	{
		printf("read:config file is empty!\n");
		exit(-1);
	}
	else if(-1 == ret)
	{
		perror("read");
		exit(-1);
	}
//	printf("buf:%s\n",buf);
	int i,j,count;
	for(i = 0,count = 0;i < conflen;i++)
	{
		if(buf[i] == '=' && count == 0)
		{
			i++;
			j = 0;
			while(1)
			{
				if(buf[i] == '>')
					break;
				conf->ip[j++] = buf[i++];
			}
			conf->ip[j] = '\0';
			count++;
		}
		if(buf[i] == '=' && count == 1)
		{
			i++;
			j = 0;
			while(1)
			{
				if(buf[i] == '>')
					break;
				conf->port[j++] = buf[i++];
			}
			conf->port[j] = '\0';
			count++;
		}
			if(buf[i] == '=' && count == 2)
		{
			i++;
			j = 0;
			while(1)
			{
				if(buf[i] == '>')
					break;
				conf->filepath[j++] = buf[i++];
			}
			conf->filepath[j] = '\0';
			count++;
		}
	}
}

void startUpServer(int* sfd,pconfig conf)
{
	//create socket
	*sfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == *sfd)
	{
		perror("socket");
		exit(-1);
	}
	//bind ip address and port
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(conf->port));
	addr.sin_addr.s_addr = inet_addr(conf->ip);
	int ret = bind(*sfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("bind");
		exit(-1);
	}
	//listen accept
	ret = listen(*sfd,MAX_CONN);
	if(-1 == ret)
	{
		perror("listen");
		exit(-1);
	}
}

void factoryInit(pfact pf,pconfig conf,int th_num,int capacity,pfunc childHandle)
{
	pf->thread = (pthread_t *)calloc(th_num,sizeof(pthread_t));
	pf->th_num = th_num;
	pthread_cond_init(&pf->cond,NULL);
	pf->capacity = capacity;
	queInit(&pf->que);
	pf->child_fun = childHandle;
	strcpy(pf->rootpath,conf->filepath);
	pf->flag = 0;
	pf->conn_num = 0;
}

void factoryStart(pfact pf)
{
	int i;
	if(0 == pf->flag)
	{
		for(i = 0;i < pf->th_num;i++)
		{
			pthread_create(&pf->thread[i],NULL,pf->child_fun,(void*)pf);
		}
	}
	pf->flag = 1;
}


