/*
 *name:pp_system.c  // achieve function in pp_system
 *author@prince_clover
 *date:2016-06-01
 *last edit date:
 */
#include "../include/pp_system.h"

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

void connServer(int* cfd,pconfig conf)
{
	//create socket
	*cfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == *cfd)
    {
        perror("socket");
        exit(-1);
    }
	//connect server
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(conf->port));
	addr.sin_addr.s_addr = inet_addr(conf->ip);
	int ret = connect(*cfd,(struct sockaddr*)&addr,sizeof(struct sockaddr));
	if(-1 == ret)
	{
		perror("connect");
		exit(-1);
	}
}
